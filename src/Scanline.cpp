#include"Scanline.h"


void Scanline::setSize(int _width, int _height) {
	width = _width;
	height = _height;
}

void Scanline::BuildTable(Obj &obj) {
	ETable.clear();
	ETable.resize(height);

	int faceNum = obj.getFaceNum();
	// printf("%d\n", faceNum);
	for (int i = 0; i < faceNum; i++) {
		Edge edge;
		float miny = INF;
		float maxy = 0;
		for (int j = 0; j < obj.face[i].size(); j++) {
			Point<float> c1 = obj.vertex[obj.face[i][j]];
			Point<float> c2 = obj.vertex[obj.face[i][(j + 1) % obj.face[i].size()]];
			if (ceil(c2.y) == ceil(c1.y)) continue;
			if (c1.y > c2.y) std::swap(c1, c2);

			edge.x = c2.x;
			edge.z = c2.z;
			edge.dy = ceil(c2.y) - ceil(c1.y);
			edge.dz = (c1.z - c2.z) / edge.dy;
			edge.dx = (c1.x - c2.x) / edge.dy;
			edge.polyID = i;
			ETable[ceil(c2.y)].push_back(edge);
			miny = std::min(miny, std::min(c1.y, c2.y));
			maxy = std::max(maxy, std::max(c1.y, c2.y));
		}
	}
}

void Scanline::UpdateActiveEdge() {
	int n2 = 0;
	for (int i = 0; i < AET.size(); ++i) {
		AET[i].dy--;
		// printf("%.3f ", AET[i].dy);
		if (AET[i].dy <= 0) continue;
		else {
			AET[i].x += AET[i].dx;
			AET[i].z += AET[i].dz;
			AET[n2] = AET[i];
			n2++;
		}
	}
	AET.erase(AET.begin() + n2, AET.end());
}

bool comp(const Edge &a, const Edge &b) {
	return a.x < b.x;
}


void Scanline::ComputeBuffer(int y, std::vector<Color> &buffer, Obj &obj) {
	std::multiset<std::pair<float, Color> > st;
	std::map<int, float> mp;
	//	std::map<int, int> fmp;
	int tot = 0;
	buffer.resize(width);
	int now = 0;
	// printf("%d\n", AET.size());
	// for(int i = 0; i < AET.size(); ++i) printf("%.3f ", AET[i].x); printf("\n");
	for (int i = 0; i < width; ++i) {
		//vector<Color> tmp;
		while (now < AET.size() && i == ceil(AET[now].x)) {
			std::pair<float, Color> tt = std::make_pair(AET[now].z, obj.color[AET[now].polyID]);
			if (mp.find(AET[now].polyID) == mp.end()) {
				mp[AET[now].polyID] = AET[now].z;
				// fmp[tot] = AET[now].polyID;
				//tot ++;
				st.insert(tt);
			}
			else {
				auto it = st.find(std::make_pair(mp[AET[now].polyID], obj.color[AET[now].polyID]));
				if (it != st.end()) st.erase(it);
				//tmp.push_back(mp[AET[now].polyID]);
			}
			now++;
		}
		if (st.size() == 0) buffer[i] = 0;
		else {
			// printf("%d\n", (*(--st.end())).second);
			buffer[i] = (*(--st.end())).second;
		}
	}
}

void Scanline::scan(Obj &obj, std::vector<std::vector<Color> > &buffer)
{
	AET.clear();
	buffer.clear();
	buffer.resize(height);

	// printf("height %d\n", height);


	for (int y = height - 1; y >= 0; y--) {
		for (int n = 0; n < (int)ETable[y].size(); n++) {
			AET.push_back(ETable[y][n]);
			// printf("%.3f ", ETable[y][n].x);
		}
		if (!AET.empty()) std::sort(AET.begin(), AET.end(), comp);
		ComputeBuffer(y, buffer[y], obj);
		UpdateActiveEdge();
	}
}