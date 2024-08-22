#include "ofApp.h"

//--------------------------------------------------------------
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetBackgroundColor(0);
    ofSetCircleResolution(50);
    twinklingSpeed = 2.0f; // 반짝임 효과의 속도 설정
    cout << "Welcome to the MST constellation drawing!" << endl;
    createConstellation(); // 별자리 만들기 시작
}

//--------------------------------------------------------------
void ofApp::update() {
    // 은은하게 반짝이는 효과를 위해 알파 값 조정
    float elapsedTime = ofGetElapsedTimef() * twinklingSpeed;
    twinklingAlpha = ofMap(sin(elapsedTime), -1, 1, 100, 200);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);

    ofSetColor(255);
    ofFill();

    float offsetX = 50; // x축으로 이동할 거리
    float offsetY = 50; // y축으로 이동할 거리
    int offsetS = 20;

    for (const auto& constellation : constellations) {
        // 별 그리기
        for (const auto& star : constellation.stars) {
            float x = star.x * 50 + offsetX; // offsetX 만큼 이동하여 그립니다.
            float y = star.y * 50 + offsetY; // offsetY 만큼 이동하여 그립니다.
            ofSetColor(255, twinklingAlpha);
            drawStar(x, y, 10, 5);
        }
        // 간선 그리기
        for (const auto& edge : constellation.edges) {
            const auto& starA = constellation.stars[edge.first];
            const auto& starB = constellation.stars[edge.second];
            float startX = starA.x * 50 + offsetX; // offsetX 만큼 이동하여 그립니다.
            float startY = starA.y * 50 + offsetY; // offsetY 만큼 이동하여 그립니다.
            float endX = starB.x * 50 + offsetX; // offsetX 만큼 이동하여 그립니다.
            float endY = starB.y * 50 + offsetY; // offsetY 만큼 이동하여 그립니다.
            ofSetColor(255);
            ofDrawLine(startX, startY, endX, endY);
        }

        // 최소 비용 출력
        ofSetColor(255);
        ofDrawBitmapString("minimum cost: " + ofToString(constellation.minCost), 10, offsetS);
        offsetS += 20;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'a') {
        createConstellation(); // 별자리 추가 생성
    }
    else if (key == 'r') {
        resetConstellations(); // 별자리 초기화
    }
    else if (key == 'q') {
        ofExit(); // 프로젝트 종료
    }
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//--------------------------------------------------------------
void ofApp::createConstellation() {
    Constellation newConstellation;

    // 별의 개수 입력 받기
    cout << "Enter the number of stars: ";
    cin >> newConstellation.numStars;

    if (newConstellation.numStars < 2) {
        cout << "Wrong input. Please enter a value greater than 1." << endl;
        return;
    }

    // 각 별의 좌표 입력 받기
    cout << "Enter the position of the stars: " << endl;
    for (int i = 0; i < newConstellation.numStars; i++) {
        float x, y;
        cin >> x >> y;
        newConstellation.stars.push_back({ x, y });
    }

    // 모든 별 좌표 쌍 간의 거리 계산
    vector<pair<float, pair<int, int>>> distances; // 거리와 해당 별 좌표 쌍을 저장하기 위한 벡터
    for (int i = 0; i < newConstellation.numStars - 1; i++) { // 모든 별의 좌표 쌍에 대해
        for (int j = i + 1; j < newConstellation.numStars; j++) { // i 다음 별부터 시작해 모든 별과의 거리 계산
            float dx = newConstellation.stars[i].x - newConstellation.stars[j].x;
            float dy = newConstellation.stars[i].y - newConstellation.stars[j].y;
            float distance = sqrt(dx * dx + dy * dy); // 거리 계산
            distances.push_back({ distance, {i, j} }); // 거리와 해당 별 좌표 쌍을 distances 벡터에 추가
            newConstellation.costs.push_back(distance); // 거리를 costs 벡터에 추가
        }
    }

    // 거리 기준으로 정렬
    sort(distances.begin(), distances.end());

    // 크루스칼 알고리즘으로 최소 비용 신장 트리 구성
    newConstellation.parent.resize(newConstellation.numStars); // parent 배열 크기 조정
    newConstellation.rank.resize(newConstellation.numStars); // rank 배열 크기 조정
    for (int i = 0; i < newConstellation.numStars; i++) { // 모든 별에 대해 반복
        makeSet(newConstellation, i); // 각 별에 대해 초기 집합 생성
    }

    int numEdges = 0; // 현재까지 추가된 간선의 수
    int idx = 0; // distances 인덱스
    while (numEdges < newConstellation.numStars - 1) { // 신장 트리를 완성할 때까지 반복
        int u = distances[idx].second.first;
        int v = distances[idx].second.second;
        int uRoot = findSet(newConstellation, u); // 첫 번째 정점의 루트 노드 찾기
        int vRoot = findSet(newConstellation, v); // 두 번째 정점의 루트 노드 찾기

        if (uRoot != vRoot) { // 두 정점이 다른 집합에 속해 있다면
            newConstellation.edges.push_back({ u, v }); // 간선 추가
            unionSets(newConstellation, uRoot, vRoot); // 두 집합을 합침
            numEdges++; // 추가된 간선의 수 증가
            newConstellation.minCost += distances[idx].first; // 최소 비용 업데이트
        }
        idx++;
    }

    // 생성된 별자리 추가
    constellations.push_back(newConstellation);
    cout << "New constellation has been created!" << endl;
}

//--------------------------------------------------------------
void ofApp::resetConstellations() {
    constellations.clear(); // 별자리 초기화
}

//--------------------------------------------------------------
void ofApp::makeSet(Constellation& constellation, int v) {
    constellation.parent[v] = v; // v를 자체적인 집합으로 설정
    constellation.rank[v] = 0; // 초기 랭크를 0으로 설정
}

//--------------------------------------------------------------
int ofApp::findSet(Constellation& constellation, int v) {
    if (v == constellation.parent[v]) { // v가 집합의 루트일 경우
        return v; // v를 반환
    }
    // v의 부모를 재귀적으로 찾아가기
    return constellation.parent[v] = findSet(constellation, constellation.parent[v]);
}

//--------------------------------------------------------------
void ofApp::unionSets(Constellation& constellation, int a, int b) {
    if (constellation.rank[a] < constellation.rank[b]) { // a의 랭크가 b의 랭크보다 작을 경우
        constellation.parent[a] = b; // a의 부모를 b로 설정
    }
    else if (constellation.rank[a] > constellation.rank[b]) { // a의 랭크가 b의 랭크보다 클 경우
        constellation.parent[b] = a; //b의 부모를 a로 설정
    }
    else { // a의 랭크와 b의 랭크가 같을 경우
        constellation.parent[b] = a; // b의 부모를 a로 설정
        constellation.rank[a]++; // a의 랭크 증가
    }
}

//--------------------------------------------------------------
void ofApp::drawStar(float x, float y, float radius, int numPoints) {
    float angle = TWO_PI / numPoints; // 점 간의 각도
    float halfAngle = angle / 2.0; // 각도의 절반

    ofBeginShape();
    for (int i = 0; i < numPoints * 2; i++) { // 별의 각 점에 대해 반복
        float r = radius * ((i % 2 == 0) ? 1.0 : 0.5); // 내접원과 외접원 크기를 번갈아가면서 설정
        float currAngle = i * angle - halfAngle; // 현재 점의 각도 계산
        float px = x + cos(currAngle) * r; // 현재 점의 x 좌표 계산
        float py = y + sin(currAngle) * r; // 현재 점의 y 좌표 계산
        ofVertex(px, py); // 계산된 좌표로 점 그리기
    }
    ofEndShape();
}