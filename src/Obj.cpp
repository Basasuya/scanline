#include"Obj.h"

Obj::Obj() {
	vertex.clear();
	normal.clear();
	face.clear();
	int n_vertex = 0;
	int n_face = 0;
	Point<float> center=Point<float>(0,0,0);
	float minx = INF;
	float miny = INF;
	float maxx = 0;
	float maxy = 0;
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
	char* str = "";
	char* sub;
	std::vector<int> temp_f; 
	Point<float> temp_c;
	str = (char*)malloc(200 * sizeof(char));
	while (file.getline(str, 200)) {
		if (str[0] == 'v'&&str[1] == ' ') {
			
			sscanf(str + 2 * sizeof(char), "%f %f %f", &temp_c.x, &temp_c.y, &temp_c.z);
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
				int pos = s.find_first_of("/");
				if (pos>0) temp_f.push_back(atoi(s.substr(0, pos).data()) - 1);
				else temp_f.push_back(atoi(sub) - 1);
				sub = strtok(NULL, " ");
			}
			face.push_back(temp_f);
			n_face++;
		}
	}
	normal.resize(face.size());
	for (int i = 0; i < n_face; i++) {
		normal.push_back((vertex[face[i][1]] - vertex[face[i][0]])*(vertex[face[i][2]] - vertex[face[i][0]]));
		normal[i] = normal[i] / std::sqrt(normal[i].x*normal[i].x + normal[i].y*normal[i].y + normal[i].z*normal[i].z);
	}

	color.resize(face.size());
	for (int i = 0; i < n_face; i++) {
		float theta = normal[i].z / sqrt(normal[i].x * normal[i].x + normal[i].y * normal[i].y + normal[i].z * normal[i].z);
		if(theta < 0) theta = -theta;
		color.push_back(255*theta);
	}
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
	
	for (int i = 1; i <= n_vertex; i++) {
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