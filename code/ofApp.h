#pragma once

#include "ofMain.h"

struct Star {
	float x; // 별의 x 좌표
	float y; // 별의 y 좌표
};

struct Edge {
	int first; // 연결된 첫 번째 별의 인덱스
	int second; // 연결된 두 번째 별의 인덱스
};

struct Constellation {
	int numStars; // 별의 개수
	vector<Star> stars; // 별들의 좌표 정보
	vector<Edge> edges; // 연결된 별들의 인덱스 정보
	vector<float> costs; // 별들 간의 거리 정보
	vector<int> parent; // 크루스칼 알고리즘에서 사용되는 부모 노드 정보
	vector<int> rank; // 크루스칼 알고리즘에서 사용되는 랭크 정보
	float minCost = 0; // 최소 비용
};

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	void createConstellation(); // 별자리 생성 함수
	void resetConstellations(); // 생성된 별자리 초기화 함수
	void makeSet(Constellation& constellation, int v); // 집합 생성 함수
	int findSet(Constellation& constellation, int v); // v가 속한 집합의 대표 노드를 찾는 함수
	void unionSets(Constellation& constellation, int a, int b); // 두 집합을 합치는 함수
	void drawStar(float x, float y, float radius, int numPoints); // 별을 그리는 함수

private:
	vector<Constellation> constellations; // 생성된 별자리들의 정보
	float twinklingSpeed; // 별자리 반짝임 효과의 속도
	float twinklingAlpha; // 별자리 반짝임 효과의 알파 값
	float offsetX; // 별자리 그리기 시 x 축으로 이동할 거리
	float offsetY; // 별자리 그리기 시 y 축으로 이동할 거리
};