#include "array.h"
#include "display.h"
#include "draw.h"
#include "light.h"
#include "matrix.h"
#include "mesh.h"
#include "texture.h"
#include "triangle.h"
#include "upng.h"
#include "vector.h"

#define M_PI 3.14159265358979323846

bool is_running = false;
int previous_frame_time = 0;

vec3_t camera_position = {0, 0, 0};
mat4_t proj_matrix;

triangle_t *triangles_to_render = NULL;

enum cull_method cull_method;
enum render_method render_method;

void setup(void) {
  render_method = RENDER_FILL_TRIANGLE;
  cull_method = CULL_BACKFACE;

  color_buffer = malloc(sizeof(uint32_t) * window_width * window_height);
  color_buffer_texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                           SDL_TEXTUREACCESS_STREAMING,
                                           window_width, window_height);

  float fov = M_PI / 3.0;
  float aspect = window_height / (float)window_width;
  float znear = 0.1;
  float zfar = 100.0;
  proj_matrix = mat4_make_perspective(fov, aspect, znear, zfar);

  load_obj_file_data("./assets/f22.obj");
  load_png_texture_data("./assets/f22.png");
}

void process_input(void) {
  SDL_Event event;
  SDL_PollEvent(&event);

  switch (event.type) {
    case SDL_QUIT:
      is_running = false;
      break;
    case SDL_KEYDOWN:
      if (event.key.keysym.sym == SDLK_ESCAPE) {
        is_running = false;
      } else if (event.key.keysym.sym == SDLK_1) {
        render_method = RENDER_WIRE_VERTEX;
      } else if (event.key.keysym.sym == SDLK_2) {
        render_method = RENDER_WIRE;
      } else if (event.key.keysym.sym == SDLK_3) {
        render_method = RENDER_FILL_TRIANGLE;
      } else if (event.key.keysym.sym == SDLK_4) {
        render_method = RENDER_FILL_TRIANGLE_WIRE;
      } else if (event.key.keysym.sym == SDLK_5) {
        render_method = RENDER_TEXTURED;
      } else if (event.key.keysym.sym == SDLK_6) {
        render_method = RENDER_TEXTURED_WIRE;
      } else if (event.key.keysym.sym == SDLK_c) {
        cull_method = CULL_BACKFACE;
      } else if (event.key.keysym.sym == SDLK_d) {
        cull_method = CULL_NONE;
      }
      break;
  }
}

void update(void) {
  int time_to_wait = FRAME_TARGET_TIME - (SDL_GetTicks() - previous_frame_time);
  if (time_to_wait > 0 && time_to_wait <= FRAME_TARGET_TIME) {
    SDL_Delay(time_to_wait);
  }

  previous_frame_time = SDL_GetTicks();

  triangles_to_render = NULL;

  mesh.rotation.x = -M_PI / 6.0;
  mesh.rotation.y += 0.02;
  mesh.translation.z = 4;

  mat4_t scale_matrix =
      mat4_make_scale(mesh.scale.x, mesh.scale.y, mesh.scale.z);
  mat4_t rotation_matrix_x = mat4_make_rotation_x(mesh.rotation.x);
  mat4_t rotation_matrix_y = mat4_make_rotation_y(mesh.rotation.y);
  mat4_t rotation_matrix_z = mat4_make_rotation_z(mesh.rotation.z);
  mat4_t translation_matrix = mat4_make_translation(
      mesh.translation.x, mesh.translation.y, mesh.translation.z);

  size_t num_faces = array_length(mesh.faces);
  for (size_t i = 0; i < num_faces; i++) {
    face_t mesh_face = mesh.faces[i];

    vec3_t face_vertices[3];
    face_vertices[0] = mesh.vertices[mesh_face.a];
    face_vertices[1] = mesh.vertices[mesh_face.b];
    face_vertices[2] = mesh.vertices[mesh_face.c];

    vec4_t transformed_vertices[3];

    for (size_t j = 0; j < 3; j++) {
      vec4_t transformed_vertex = vec4_from_vec3(face_vertices[j]);

      mat4_t world_matrix = mat4_indentity();
      world_matrix = mat4_mul_mat4(scale_matrix, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_x, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_y, world_matrix);
      world_matrix = mat4_mul_mat4(rotation_matrix_z, world_matrix);
      world_matrix = mat4_mul_mat4(translation_matrix, world_matrix);

      transformed_vertex = mat4_mul_vec4(world_matrix, transformed_vertex);
      transformed_vertices[j] = transformed_vertex;
    }

    vec3_t vector_a = vec3_from_vec4(transformed_vertices[0]);
    vec3_t vector_b = vec3_from_vec4(transformed_vertices[1]);
    vec3_t vector_c = vec3_from_vec4(transformed_vertices[2]);

    vec3_t vector_ab = vec3_normalize(vec3_sub(vector_b, vector_a));
    vec3_t vector_ac = vec3_normalize(vec3_sub(vector_c, vector_a));

    vec3_t normal = vec3_normalize(vec3_cross(vector_ab, vector_ac));

    vec3_t camera_ray = vec3_sub(camera_position, vector_a);

    float dot_normal_camera = vec3_dot(normal, camera_ray);

    if (cull_method == CULL_BACKFACE && dot_normal_camera < 0) {
      continue;
    }

    vec4_t projected_points[3];
    for (size_t j = 0; j < 3; j++) {
      projected_points[j] =
          mat4_mul_vec4_project(proj_matrix, transformed_vertices[j]);

      projected_points[j].x *= window_width / 2;
      projected_points[j].y *= -window_height / 2;

      projected_points[j].x += window_width / 2;
      projected_points[j].y += window_height / 2;
    }

    float light_intensity_factor =
        -vec3_dot(normal, vec3_normalize(light.direction));
    color_t triangle_color =
        light_apply_intensity(mesh_face.color, light_intensity_factor);
    float triangle_average_depth =
        (transformed_vertices[0].z + transformed_vertices[1].z +
         transformed_vertices[2].z) /
        3.0;

    triangle_t projected_triangle = {
        .points =
            {
                {projected_points[0].x, projected_points[0].y,
                 projected_points[0].z, projected_points[0].w},
                {projected_points[1].x, projected_points[1].y,
                 projected_points[1].z, projected_points[1].w},
                {projected_points[2].x, projected_points[2].y,
                 projected_points[2].z, projected_points[2].w},
            },
        .texcoords =
            {
                {mesh_face.a_uv.u, mesh_face.a_uv.v},
                {mesh_face.b_uv.u, mesh_face.b_uv.v},
                {mesh_face.c_uv.u, mesh_face.c_uv.v},
            },
        .color = triangle_color,
        .average_depth = triangle_average_depth};

    array_push(triangles_to_render, projected_triangle);

    int num_triangles = array_length(triangles_to_render);
    for (size_t i = 0; i < num_triangles; i++) {
      for (size_t j = i; j < num_triangles; j++) {
        if (triangles_to_render[i].average_depth <
            triangles_to_render[j].average_depth) {
          triangle_t temp = triangles_to_render[i];
          triangles_to_render[i] = triangles_to_render[j];
          triangles_to_render[j] = temp;
        }
      }
    }
  }
}

void render(void) {
  draw_grid(10, 0xff353535);

  int num_triangles = array_length(triangles_to_render);
  for (size_t i = 0; i < num_triangles; i++) {
    triangle_t triangle = triangles_to_render[i];

    if (render_method == RENDER_FILL_TRIANGLE ||
        render_method == RENDER_FILL_TRIANGLE_WIRE) {
      draw_filled_triangle(triangle);
    }

    if (render_method == RENDER_TEXTURED ||
        render_method == RENDER_TEXTURED_WIRE) {
      draw_textured_triangle(triangle, mesh_texture);
    }

    if (render_method == RENDER_WIRE || render_method == RENDER_WIRE_VERTEX ||
        render_method == RENDER_FILL_TRIANGLE_WIRE ||
        render_method == RENDER_TEXTURED_WIRE) {
      draw_triangle(triangle, 0xfff59201);
    }

    if (render_method == RENDER_WIRE_VERTEX) {
      draw_rect(triangle.points[0].x - 3, triangle.points[0].y - 3, 6, 6,
                0xfff59201);
      draw_rect(triangle.points[1].x - 3, triangle.points[1].y - 3, 6, 6,
                0xfff59201);
      draw_rect(triangle.points[2].x - 3, triangle.points[2].y - 3, 6, 6,
                0xfff59201);
    }
  }

  array_free(triangles_to_render);

  render_color_buffer();
  clear_color_buffer(0xff656565);
  SDL_RenderPresent(renderer);
}

void free_resources(void) {
  free(color_buffer);
  upng_free(png_texture);
  array_free(mesh.faces);
  array_free(mesh.vertices);
}

int main(int argc, char *argv[]) {
  is_running = initialize_window();

  setup();

  while (is_running) {
    process_input();
    update();
    render();
  }

  destroy_window();
  free_resources();

  return 0;
}
