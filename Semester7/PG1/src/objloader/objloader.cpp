#include "objloader.hpp"

#include "../utils/math.hpp"
#include "../utils/utils.hpp"
#include <cstring>
#include <map>

bool MaterialExists(std::vector<Material *> &materials, char *material_name) {
  for (std::vector<Material *>::const_iterator iter = materials.begin();
       iter != materials.end(); ++iter) {
    Material *material = *iter;

    if (material->get_name().compare(material_name) == 0) {
      return true;
    }
  }

  return false;
}

Texture *TextureProxy(const std::string &full_name,
                      std::map<std::string, Texture *> &already_loaded_textures,
                      const int flip = -1, const bool single_channel = false) {
  std::map<std::string, Texture *>::iterator already_loaded_texture =
      already_loaded_textures.find(full_name);
  Texture *texture = NULL;
  if (already_loaded_texture != already_loaded_textures.end()) {
    texture = already_loaded_texture->second;
  } else {
    texture = new Texture(full_name.c_str()); // , flip, single_channel);
    already_loaded_textures[full_name] = texture;
  }

  return texture;
}

/*! \fn LoadMTL( const char * file_name, const char * path, std::vector<Material
*> & materials )
\brief Na�te materi�ly z MTL souboru \a file_name.
Soubor \a file_name se mus� nach�zet v cest� \a path. Na�ten� materi�ly budou
vr�ceny p�es pole \a materials.
\param file_name n�zev MTL souboru v�etn� p��pony.
\param path cesta k zadan�mu souboru.
\param materials pole materi�l�, do kter�ho se budou ukl�dat na�ten� materi�ly.
*/
int LoadMTL(const char *file_name, const char *path,
            std::vector<Material *> &materials) {
  // otev�en� soouboru
  FILE *file = fopen(file_name, "rt");
  if (file == NULL) {
    printf("File %s not found.\n", file_name);

    return -1;
  }

  // na�ten� cel�ho souboru do pam�ti
  size_t file_size = static_cast<size_t>(GetFileSize64(file_name));
  char *buffer =
      new char[file_size +
               1]; // +1 proto�e budeme za posledn� na�ten� byte d�vat NULL
  char *buffer_backup = new char[file_size + 1];

  printf("Loading materials from '%s' (%0.1f KB)...\n", file_name,
         file_size / 1024.0f);

  size_t number_of_items_read = fread(buffer, sizeof(*buffer), file_size, file);

  // otestujeme korektnost na�ten� dat
  if (!feof(file) && (number_of_items_read != file_size)) {
    printf("Unexpected end of file encountered.\n");

    fclose(file);
    file = NULL;

    return -1;
  }

  buffer[number_of_items_read] = 0; // zajist�me korektn� ukon�en� �et�zce

  fclose(file); // ukon��me pr�ci se souborem
  file = NULL;

  memcpy(buffer_backup, buffer, file_size + 1); // z�loha bufferu

  printf("Done.\n\n");

  printf("Parsing mesh data...\n");

  char material_name[128] = {0};
  char image_file_name[256] = {0};

  const char delim[] = "\n";
  char *line = strtok(buffer, delim);

  std::map<std::string, Texture *> already_loaded_textures;

  Material *material = NULL;

  // --- na��t�n� v�ech materi�l� ---
  while (line != NULL) {
    if (line[0] != '#') {
      if (strstr(line, "newmtl") == line) {
        if (material != NULL) {
          material->set_name(material_name);
          if (!MaterialExists(materials, material_name)) {
            materials.push_back(material);
            printf("\r%I64u material(s)\t\t", materials.size());
          }
        }
        material = NULL;

        sscanf(line, "%*s %s", &material_name);
        // printf( "material name=%s\n", material_name );

        material = new Material();
      } else {
        char *tmp = Trim(line);
        if (strstr(tmp, "Ka") == tmp) // ambient color of the material
        {
          sscanf(tmp, "%*s %f %f %f", &material->ambient.x,
                 &material->ambient.y, &material->ambient.z);
        }
        if (strstr(tmp, "Kd") == tmp) // diffuse color of the material
        {
          sscanf(tmp, "%*s %f %f %f", &material->diffuse.x,
                 &material->diffuse.y, &material->diffuse.z);
        }
        if (strstr(tmp, "Ks") == tmp) // specular color of the material
        {
          sscanf(tmp, "%*s %f %f %f", &material->specular.x,
                 &material->specular.y, &material->specular.z);
        }
        if (strstr(tmp, "Ke") == tmp) // emission color of the material
        {
          sscanf(tmp, "%*s %f %f %f", &material->emission.x,
                 &material->emission.y, &material->emission.z);
        }
        if (strstr(tmp, "Ns") == tmp) // specular coefficient
        {
          sscanf(tmp, "%*s %f", &material->shininess);
        }
        if (strstr(tmp, "map_Kd") == tmp) // diffuse map
        {
          sscanf(tmp, "%*s %s", image_file_name);
          std::string full_name = std::string(path).append(image_file_name);
          material->set_texture(
              Material::kDiffuseMapSlot,
              TextureProxy(full_name, already_loaded_textures));
        }
        if (strstr(tmp, "map_Ks") == tmp) // specular map
        {
          sscanf(tmp, "%*s %s", image_file_name);
          std::string full_name = std::string(path).append(image_file_name);
          material->set_texture(
              Material::kSpecularMapSlot,
              TextureProxy(full_name, already_loaded_textures));
        }
        if (strstr(tmp, "map_bump") == tmp) // normal map
        {
          float bm = 0;
          sscanf(tmp, "%*s %*s %f %s", &bm, image_file_name);
          std::string full_name = std::string(path).append(image_file_name);
          material->set_texture(
              Material::kNormalMapSlot,
              TextureProxy(full_name, already_loaded_textures));
        }
        if (strstr(tmp, "map_D") == tmp) // opacity map
        {
          sscanf(tmp, "%*s %s", image_file_name);
          std::string full_name = std::string(path).append(image_file_name);
          material->set_texture(
              Material::kOpacityMapSlot,
              TextureProxy(full_name, already_loaded_textures, -1, true));
        }
        if (strstr(tmp, "shader") == tmp) // shader model
        {
          sscanf(tmp, "%*s %d", &material->shader);
        }
      }
    }

    line = strtok(NULL, delim); // na�ten� dal��ho ��dku
  }

  if (material != NULL) {
    material->set_name(material_name);
    materials.push_back(material);
    printf("\r%I64u material(s)\t\t", materials.size());
  }
  material = NULL;

  // memcpy( buffer, buffer_backup, file_size + 1 ); // obnoven� bufferu po
  // �innosti strtok
  SAFE_DELETE_ARRAY(buffer_backup);
  SAFE_DELETE_ARRAY(buffer);

  printf("\n");

  return 0;
}

int LoadOBJ(const char *file_name, std::vector<Surface *> &surfaces,
            std::vector<Material *> &materials, const bool flip_yz,
            const glm::vec3 default_color) {
  // otev�en� soouboru
  FILE *file = fopen(file_name, "rt");
  if (file == NULL) {
    printf("File %s not found.\n", file_name);

    return -1;
  }

  // cesta k zadan�mu souboru
  char path[128] = {""};
  const char *tmp = strrchr(file_name, '/');
  if (tmp != NULL) {
    memcpy(path, file_name, sizeof(char) * (tmp - file_name + 1));
  }

  // na�ten� cel�ho souboru do pam�ti
  /*const long long*/ size_t file_size =
      static_cast<size_t>(GetFileSize64(file_name));
  char *buffer =
      new char[file_size +
               1]; // +1 proto�e budeme za posledn� na�ten� byte d�vat NULL
  char *buffer_backup = new char[file_size + 1];

  printf("Loading model from '%s' (%0.1f MB)...\n", file_name,
         file_size / sqr(1024.0f));

  size_t number_of_items_read = fread(buffer, sizeof(*buffer), file_size, file);

  // otestujeme korektnost na�ten� dat
  if (!feof(file) && (number_of_items_read != file_size)) {
    printf("Unexpected end of file encountered.\n");

    fclose(file);
    file = NULL;

    return -1;
  }

  buffer[number_of_items_read] = 0; // zajist�me korektn� ukon�en� �et�zce

  fclose(file); // ukon��me pr�ci se souborem
  file = NULL;

  memcpy(buffer_backup, buffer, file_size + 1); // z�loha bufferu

  printf("Done.\n\n");

  printf("Parsing material data...\n");

  char material_library[128] = {0};

  std::vector<std::string> material_libraries;

  const char delim[] = "\n";
  char *line = strtok(buffer, delim);

  // --- na��t�n� v�ech materi�lov�ch knihoven, prvn� pr�chod ---
  while (line != NULL) {
    switch (line[0]) {
    case 'm': // mtllib
    {
      sscanf(line, "%*s %s", &material_library);
      printf("Material library: %s\n", material_library);
      material_libraries.push_back(
          std::string(path).append(std::string(material_library)));
    } break;
    }

    line = strtok(NULL, delim); // na�ten� dal��ho ��dku
  }

  memcpy(buffer, buffer_backup,
         file_size + 1); // obnoven� bufferu po �innosti strtok

  for (int i = 0; i < static_cast<int>(material_libraries.size()); ++i) {
    LoadMTL(material_libraries[i].c_str(), path, materials);
  }

  std::vector<glm::vec3> vertices; // cel� jeden soubor
  std::vector<glm::vec3> per_vertex_normals;
  std::vector<Coord2f> texture_coords;

  line = strtok(buffer, delim);

  // --- na��t�n� v�ech sou�adnic, druh� pr�chod ---
  while (line != NULL) {
    switch (line[0]) {
    case 'v': // seznam vrchol�, norm�l nebo texturovac�ch sou�adnic aktu�ln�
              // skupiny
    {
      switch (line[1]) {
      case ' ': // vertex
      {
        glm::vec3 vertex;
        if (flip_yz) {
          // float x, y, z;
          sscanf(line, "%*s %f %f %f", &vertex.x, &vertex.z, &vertex.y);
          vertex.y *= -1;
        } else {
          sscanf(line, "%*s %f %f %f", &vertex.x, &vertex.y, &vertex.z);
        }

        vertices.push_back(vertex);
      } break;

      case 'n': // norm�la vertexu
      {
        glm::vec3 normal;
        if (flip_yz) {
          // float x, y, z;
          sscanf(line, "%*s %f %f %f", &normal.x, &normal.z, &normal.y);
          normal.y *= -1;
        } else {
          sscanf(line, "%*s %f %f %f", &normal.x, &normal.y, &normal.z);
        }
        normal = glm::normalize(normal);
        per_vertex_normals.push_back(normal);
      } break;

      case 't': // texturovac� sou�adnice
      {
        Coord2f texture_coord;
        float z = 0;
        sscanf(line, "%*s %f %f %f", &texture_coord.u, &texture_coord.v, &z);
        texture_coords.push_back(texture_coord);
      } break;
      }
    } break;
    }

    line = strtok(NULL, delim); // na�ten� dal��ho ��dku
  }

  memcpy(buffer, buffer_backup,
         file_size + 1); // obnoven� bufferu po �innosti strtok

  printf("%I64u vertices, %I64u normals and %I64u texture coords.\n",
         vertices.size(), per_vertex_normals.size(), texture_coords.size());

  /// buffery pro na��t�n� �et�zc�
  char group_name[128];
  char material_name[128];
  char vertices_indices[4][8 * 3 + 2]; // pomocn� �et�zec pro na��t�n� index� a�
                                       // 4 x "v/vt/vn"
  char vertex_indices[3][8]; // pomocn� �et�zec jednotliv�ch index� "v", "vt" a
                             // "vn"

  std::vector<Vertex> face_vertices; // pole v�ech vertex� pr�v� na��tan� face

  int no_surfaces = 0; // po�et na�ten�ch ploch

  line = strtok(buffer, delim); // reset

  // --- na��t�n� jednotliv�ch objekt� (group), t�et� pr�chod ---
  while (line != NULL) {
    switch (line[0]) {
    case 'g': // group
    {
      if (face_vertices.size() > 0) {
        surfaces.push_back(
            BuildSurface(std::string(group_name), face_vertices));
        printf("\r%I64u group(s)\t\t", surfaces.size());
        ++no_surfaces;
        face_vertices.clear();

        for (int i = 0; i < static_cast<int>(materials.size()); ++i) {
          if (materials[i]->get_name().compare(material_name) == 0) {
            Surface *s = *--surfaces.end();
            s->set_material(materials[i]);
            break;
          }
        }
      }

      sscanf(line, "%*s %s", &group_name);
      // printf( "Group name: %s\n", group_name );
    } break;

    case 'u': // usemtl
    {
      sscanf(line, "%*s %s", &material_name);
      // printf( "Material name: %s\n", material_name );
    } break;

    case 'f': // face
    {
      // ! p�edpokl�d�me pouze troj�heln�ky !
      // ! p�edpokl�d�me vyu�it� v�ech t�� polo�ek v/vt/vn !
      int no_slashes = 0;
      for (int i = 0; i < int(strlen(line)); ++i) {
        if (line[i] == '/') {
          ++no_slashes;
        }
      }
      switch (no_slashes) {
      case 2 * 3: // triangles
        sscanf(line, "%*s %s %s %s", &vertices_indices[0], &vertices_indices[1],
               &vertices_indices[2]);
        break;

      case 2 * 4: // quadrilaterals
        sscanf(line, "%*s %s %s %s %s", &vertices_indices[0],
               &vertices_indices[1], &vertices_indices[2],
               &vertices_indices[3]);
        break;
      }

      // TODO smoothing groups

      for (int i = 0; i < 3; ++i) {
        if (strstr(vertices_indices[i], "//")) {
          sscanf(vertices_indices[i], "%[0-9]//%[0-9]", &vertex_indices[0],
                 &vertex_indices[2]);
          vertex_indices[1][0] = 0;
        } else {
          sscanf(vertices_indices[i], "%[0-9]/%[0-9]/%[0-9]",
                 &vertex_indices[0], &vertex_indices[1], &vertex_indices[2]);
        }

        const int vertex_index = atoi(vertex_indices[0]) - 1;
        const int texture_coord_index = atoi(vertex_indices[1]) - 1;
        const int per_vertex_normal_index = atoi(vertex_indices[2]) - 1;

        if (texture_coord_index >= 0) {
          face_vertices.push_back(
              Vertex(vertices[vertex_index],
                     per_vertex_normals[per_vertex_normal_index], default_color,
                     &texture_coords[texture_coord_index]));
        } else {
          face_vertices.push_back(Vertex(
              vertices[vertex_index],
              per_vertex_normals[per_vertex_normal_index], default_color));
        }
      }

      if (no_slashes == 2 * 4) {
        const int i[] = {0, 2, 3};
        for (int j = 0; j < 3; ++j) {
          sscanf(vertices_indices[i[j]], "%[0-9]/%[0-9]/%[0-9]",
                 &vertex_indices[0], &vertex_indices[1], &vertex_indices[2]);

          const int vertex_index = atoi(vertex_indices[0]) - 1;
          const int texture_coord_index = atoi(vertex_indices[1]) - 1;
          const int per_vertex_normal_index = atoi(vertex_indices[2]) - 1;

          face_vertices.push_back(
              Vertex(vertices[vertex_index],
                     per_vertex_normals[per_vertex_normal_index], default_color,
                     &texture_coords[texture_coord_index]));
        }
      }
    } break;
    }

    line = strtok(NULL, delim); // na�ten� dal��ho ��dku
  }

  if (face_vertices.size() > 0) {
    surfaces.push_back(BuildSurface(std::string(group_name), face_vertices));
    printf("\r%I64u group(s)\t\t", surfaces.size());
    ++no_surfaces;
    face_vertices.clear();

    for (int i = 0; i < static_cast<int>(materials.size()); ++i) {
      if (materials[i]->get_name().compare(material_name) == 0) {
        Surface *s = *--surfaces.end();
        s->set_material(materials[i]);
        break;
      }
    }
  }

  texture_coords.clear();
  per_vertex_normals.clear();
  vertices.clear();

  SAFE_DELETE_ARRAY(buffer_backup);
  SAFE_DELETE_ARRAY(buffer);

  printf("\nDone.\n\n");

  return no_surfaces;
}