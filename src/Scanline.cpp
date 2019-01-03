#include"Scanline.h"


void Scanline::setSize(int _width, int _height){
    width = _width;
    height = _height;
}

Color Scanline::getColor(Point<float> normal) {
	if (normal.z < 0) normal.z = -normal.z;
	if (normal.x < 0) normal.x = -normal.x;
	if (normal.y < 0) normal.y = -normal.y;
	float theta = normal.z / sqrt(normal.x*normal.x + normal.y*normal.y + normal.z*normal.z);
	return 255 * theta;
}

void Scanline::BuildTable(Obj &obj) {
	ETable.clear();
	PTable.clear();
	ETable.resize(height);
	PTable.resize(height);
	
	int faceNum = obj.getFaceNum();
	for (int i = 0; i < faceNum; i++) {
		Edge edge;
		float miny = INF;
		float maxy = 0;
		for (int j = 0; j < obj.face[i].size();j++) {
			Point<float> c1 = obj.vertex[obj.face[i][j]];
			Point<float> c2 = obj.vertex[obj.face[i][(j + 1) % obj.face[i].size()]];
			if (c1.y == c2.y) continue;
			if (c1.y>c2.y) std::swap(c1, c2);

			edge.x = c2.x;
			edge.dy = ceil(c2.y) - ceil(c1.y);
			edge.dx = (c1.x - c2.x) / edge.dy;
			edge.polyID = i;
			ETable[ceil(c2.y)].push_back(edge);

			miny = std::min(miny, c1.y);
			maxy = std::max(maxy, c1.y);
		}
		if (maxy == 0) continue;
		Poly poly;
		poly.polyID = i;
		poly.dy = ceil(maxy) - ceil(miny);
		poly.a = obj.normal[i].x;
		poly.b = obj.normal[i].y;
		poly.c = obj.normal[i].z;
		Point<float> temp = obj.vertex[obj.face[i][0]];
		poly.d = -(poly.a*temp.x + poly.b*temp.y + poly.c*temp.z);
		poly.flag = 0;
		poly.color = getColor(obj.normal[i]);
		PTable[ceil(maxy)].push_back(poly);
	}
}

void Scanline::UpdateActiveEdge() {
	int n2 = 0;
	for (auto it = AET.begin(); it != AET.end(); it++) {
		(*it).dy--;
		if ((*it).dy == 0) continue;
		else {
			(*it).x += (*it).dx;
			*(AET.begin()+n2) = *(it);
			n2++;
		}
	}
	AET.resize(n2);
}

void Scanline::UpdateActivePoly() {
	int n2 = 0;
	for (auto it = APT.begin(); it != APT.end(); it++) {
		(*it).dy--;
		if ((*it).dy == 0) continue;
		else {
			*(APT.begin() + n2) = *(it);
			n2++;
		}
	}
	APT.resize(n2);
}

bool comp(const Edge &a, const Edge &b) {
	return a.x < b.x;
}

int Scanline::UpdateFlagByID(int id) {
	for (auto it = APT.begin(); it != APT.end(); it++) {
		if ((*it).polyID == id) {
			(*it).flag = !(*it).flag;
			return (*it).flag;
		}
	}
}

Color Scanline::pixelByID(int count, float x, float y) {
	Color col = 0;
	y = y - 0.5;
	float maxz = -INF, z;
 
	for (std::vector<Poly>::iterator it = APT.begin(); count > 0 && it != APT.end(); it++) {
		if ((*it).flag) {
			z = -((*it).a*x + (*it).b*y + (*it).d) / (*it).c;
			if (z > maxz) {
				maxz = z;
				col = (*it).color;
			}
			count--;
		}
	}
	return col;
}

void Scanline::ComputeBuffer(int y, std::vector<Color> &buffer, Obj &obj) {
	// std::set<int> st;
	// std::map<int, int> mp;
	// std::map<int, int> fmp;
	// int tot = 0;

	// int now = 0;
	// for(int i = 0; i < width; ++i) {
	// 	while( now < AET.size() && i == ceil(AET[now].x) ) {
	// 		if(mp.find(AET[now].polyID) == mp.end()) {
	// 			mp[AET[now].polyID] = tot;
	// 			fmp[tot] = AET[now].polyID;
	// 			tot ++;
	// 			st.insert(mp[AET[now].polyID]);
	// 		} else {
	// 			st.erase(mp[AET[now].polyID]);
	// 		}			
	// 		now ++;
	// 	}
	// 	if(st.size() == 0) buffer[i] = 0;
	// 	else buffer[i] = obj.color[fmp[*(--st.end())]];
	// }

	float left = 0, right;
	int count = 0;
	buffer.resize(width);
	for (int n = 0; n < (int)AET.size(); n++) {
		right = AET[n].x;
		if (left >= right) {
			count = UpdateFlagByID(AET[n].polyID) ? count + 1 : count - 1;
			continue;
		}
		Color col = 0;
		if (count == 0) {
			for(int pixel=left;left<right;left++) {
				if(left >= width) continue;
				buffer[left]=0;
			}
		}
		else if (count == 1)
		{
			col = pixelByID(count, left, y);
 
			for(float pixel=left;left<right;left++) {
				if(left >= width) continue;
				buffer[left] = col;
			}
		}
		else
		{
			std::vector<endpoint> points;
			Poly p1, p2;
			std::vector<float> cross;
 
			for (int i = 0; i < APT.size();i++)
			{
				if (points.size() == count) break;
				if (APT[i].flag)
				{
					points.push_back( endpoint(float(-(APT[i].a*left + APT[i].b*(y + 0.5) + APT[i].d) / APT[i].c),
									    float(-(APT[i].a*right + APT[i].b*(y + 0.5) + APT[i].d) / APT[i].c),
									    i ));
				}
			}
 
			for (int i = 0; i < points.size(); i++)
			{
				for (int j = i+1;j < points.size(); j++)
				{
					if ((points[i].zl - points[j].zl) * (points[i].zr - points[j].zr) < 0)
					{
						p1 = APT[points[i].order];
						p2 = APT[points[j].order];
						cross.push_back((p2.c*p1.d + p2.c*p1.b*y - p1.c*p2.d - p1.c*p2.b*y) / (p1.c*p2.a - p2.c*p1.a));
					}
				}
			}
			cross.push_back(right);
			std::sort(cross.begin(), cross.end());
			for (int i = 0; i < cross.size();i++)
			{
				right = cross[i];
				col = pixelByID(count, left, y);
 
				for(float pixel=left;left<right;left++)
				{
					if(left >= width) continue;
					buffer[left] = col;
				}
			}
		}
		count = UpdateFlagByID(AET[n].polyID) ? count + 1 : count - 1;
		left = right;
	}
 
	for (float pixel = left; left < width; left++)
	{
		buffer[left] = 0;
	}
}

void Scanline::scan(Obj &obj, std::vector<std::vector<Color> > &buffer)
{
	AET.clear();
	APT.clear();
	buffer.clear();
	buffer.resize(height);
	
	printf("height %d\n", height);


	for (int y = height-1; y >= 0; y--) {
		printf("%d\n", y);
		for (int n = 0; n < (int)ETable[y].size(); n++) {
			AET.push_back(ETable[y][n]);
		}
		for (int n = 0; n < (int)PTable[y].size(); n++) {
			APT.push_back(PTable[y][n]);
		}
		if(!AET.empty()) std::sort(AET.begin(), AET.end(), comp);
		ComputeBuffer(y, buffer[y], obj);

		UpdateActivePoly();
		UpdateActiveEdge();
	}
}