#include"Obj.h"

Obj::Obj() {
	vertex.clear();
	normal.clear();
	face.clear();
	color.clear();
	n_vertex = 0;
	n_face = 0;
	center=Point<float>(0,0,0);
	minx = INF;
	miny = INF;
	maxx = 0;
	maxy = 0;
}

int Obj::getFaceNum() {
	return n_face;
}

void Obj::load(std::string file_path) {
	std::ifstream file(file_path);
	if (!file.is_open()) {
		printf("the path is unavailable\n");
		exit(0);
	}
	char* str;
	char* sub;
	std::vector<int> temp_f; 
	Point<float> temp_c;
	str = (char*)malloc(200 * sizeof(char));
	while (file.getline(str, 200)) {
		if (str[0] == 'v'&&str[1] == ' ') {
			
			sscanf(str + 1 * sizeof(char), "%f %f %f", &temp_c.x, &temp_c.y, &temp_c.z);
			vertex.push_back(temp_c);
			n_vertex++;
			minx = std::min(minx, temp_c.x);
			maxx = std::max(maxx, temp_c.x);
			miny = std::min(miny, temp_c.y);
			maxy = std::max(maxy, temp_c.y);
		} else if (str[0] == 'f') {
			temp_f.clear();
			sub = strtok(str, " ");
			sub = strtok(NULL, " ");
			while (sub) {
				std::string s(sub);
				int pos = -1;
				for(int j = 0; j < s.length(); ++j) {
					if(s[j] == '/') {
						pos = j;
						break;
					}
				}
				if (pos>0) temp_f.push_back(atoi(s.substr(0, pos).data()) - 1);
				else temp_f.push_back(atoi(sub) - 1);
				sub = strtok(NULL, " ");
			}
			// for(int i = 0; i < temp_f.size(); ++i) printf("%d ", temp_f[i]);
			face.push_back(temp_f);
			n_face++;
		}
	}
	normal.resize(face.size());
	for (int i = 0; i < n_face; i++) {
		normal[i] = ((vertex[face[i][1]] - vertex[face[i][0]]) * (vertex[face[i][2]] - vertex[face[i][0]]));
		normal[i] = normal[i] / std::sqrt(normal[i].x*normal[i].x + normal[i].y*normal[i].y + normal[i].z * normal[i].z);
	}

	color.resize(face.size());
	for (int i = 0; i < n_face; i++) {
		float theta = normal[i].z / sqrt(normal[i].x * normal[i].x + normal[i].y * normal[i].y + normal[i].z * normal[i].z);
		// printf("%.3f %.3f %.3f %.3f: ", theta, normal[i].x, normal[i].y, normal[i].z);
		if(theta < 0) theta = -theta;
		color[i] = 255*theta;
	}

	// for(int i = 0; i < n_face; ++i) printf("%d ", color[i]); printf("\n");
	// exit(0);
}

void Obj::scale(int width, int height) {
	float scale = 1;
	scale = maxx - minx;
	scale = (maxy - miny) > scale ? maxy - miny : scale;
	if (width<height) {
		if (width>300)
			scale = (width - 200) / scale;
		else scale = width / scale;
	} else {
		if (height>300)
			scale = (height - 200) / scale;
		else scale = height / scale;
	}
	
	for (int i = 0; i < n_vertex; i++) {
		vertex[i] = (vertex[i] - center) * scale;
		vertex[i].x = vertex[i].x  + width / 2;
		vertex[i].y = vertex[i].y  + height / 2;
	}
	minx = (minx - center.x)*scale + width / 2;
	maxx = (maxx - center.x)*scale + width / 2;
	miny = (miny - center.y)*scale + height / 2;
	maxy = (maxy - center.y)*scale + height / 2;

	center = Point<float>(width / 2, height / 2, 0);
}