#include "mesh.h"
#include "array.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

mesh_t mesh = {.vertices = NULL,
               .faces = NULL,
               .rotation = {0, 0, 0},
               .scale = {1, 1, 1},
               .translation = {0, 0, 0}};

vec3_t cube_vertices[N_CUBE_VERTICES] = {
    {-1, -1, -1}, // 1
    {-1, 1, -1},  // 2
    {1, 1, -1},   // 3
    {1, -1, -1},  // 4
    {1, 1, 1},    // 5
    {1, -1, 1},   // 6
    {-1, 1, 1},   // 7
    {-1, -1, 1}   // 8
};

face_t cube_faces[N_CUBE_FACES] = {
    // front
    {1, 2, 3, 0xFFFFFFFF},
    {1, 3, 4, 0xFFFFFFFF},
    // right
    {4, 3, 5, 0xFFFFFFFF},
    {4, 5, 6, 0xFFFFFFFF},
    // back
    {6, 5, 7, 0xFFFFFFFF},
    {6, 7, 8, 0xFFFFFFFF},
    // left
    {8, 7, 2, 0xFFFFFFFF},
    {8, 2, 1, 0xFFFFFFFF},
    // top
    {2, 7, 5, 0xFFFFFFFF},
    {2, 5, 3, 0xFFFFFFFF},
    // bottom
    {6, 8, 1, 0xFFFFFFFF},
    {6, 1, 4, 0xFFFFFFFF},
};

void load_cube_mesh_data(void) {
  for (size_t i = 0; i < N_CUBE_VERTICES; i++) {
    vec3_t cube_vertex = cube_vertices[i];
    array_push(mesh.vertices, cube_vertex);
  }

  for (size_t i = 0; i < N_CUBE_FACES; i++) {
    face_t cube_face = cube_faces[i];
    array_push(mesh.faces, cube_face);
  }
}

void load_obj_file_data(const char *filename) {
  FILE *file = fopen(filename, "r");

  char line[1024];

  while (fgets(line, 1024, file)) {
    if (strncmp(line, "v ", 2) == 0) {
      vec3_t vertex;
      sscanf(line, "v %f %f %f", &vertex.x, &vertex.y, &vertex.z);
      array_push(mesh.vertices, vertex);
    }

    if (strncmp(line, "f ", 2) == 0) {
      face_t face;
      sscanf(line, "f %d/%*d/%*d %d/%*d/%*d %d/%*d/%*d", &face.a, &face.b,
             &face.c);
      face.color = 0x4c5647ff;
      array_push(mesh.faces, face);
    }
  }
}
