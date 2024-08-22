#include "ofApp.h"

//--------------------------------------------------------------
#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
    ofSetBackgroundColor(0);
    ofSetCircleResolution(50);
    twinklingSpeed = 2.0f; // ��¦�� ȿ���� �ӵ� ����
    cout << "Welcome to the MST constellation drawing!" << endl;
    createConstellation(); // ���ڸ� ����� ����
}

//--------------------------------------------------------------
void ofApp::update() {
    // �����ϰ� ��¦�̴� ȿ���� ���� ���� �� ����
    float elapsedTime = ofGetElapsedTimef() * twinklingSpeed;
    twinklingAlpha = ofMap(sin(elapsedTime), -1, 1, 100, 200);
}

//--------------------------------------------------------------
void ofApp::draw() {
    ofBackground(0);

    ofSetColor(255);
    ofFill();

    float offsetX = 50; // x������ �̵��� �Ÿ�
    float offsetY = 50; // y������ �̵��� �Ÿ�
    int offsetS = 20;

    for (const auto& constellation : constellations) {
        // �� �׸���
        for (const auto& star : constellation.stars) {
            float x = star.x * 50 + offsetX; // offsetX ��ŭ �̵��Ͽ� �׸��ϴ�.
            float y = star.y * 50 + offsetY; // offsetY ��ŭ �̵��Ͽ� �׸��ϴ�.
            ofSetColor(255, twinklingAlpha);
            drawStar(x, y, 10, 5);
        }
        // ���� �׸���
        for (const auto& edge : constellation.edges) {
            const auto& starA = constellation.stars[edge.first];
            const auto& starB = constellation.stars[edge.second];
            float startX = starA.x * 50 + offsetX; // offsetX ��ŭ �̵��Ͽ� �׸��ϴ�.
            float startY = starA.y * 50 + offsetY; // offsetY ��ŭ �̵��Ͽ� �׸��ϴ�.
            float endX = starB.x * 50 + offsetX; // offsetX ��ŭ �̵��Ͽ� �׸��ϴ�.
            float endY = starB.y * 50 + offsetY; // offsetY ��ŭ �̵��Ͽ� �׸��ϴ�.
            ofSetColor(255);
            ofDrawLine(startX, startY, endX, endY);
        }

        // �ּ� ��� ���
        ofSetColor(255);
        ofDrawBitmapString("minimum cost: " + ofToString(constellation.minCost), 10, offsetS);
        offsetS += 20;
    }
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
    if (key == 'a') {
        createConstellation(); // ���ڸ� �߰� ����
    }
    else if (key == 'r') {
        resetConstellations(); // ���ڸ� �ʱ�ȭ
    }
    else if (key == 'q') {
        ofExit(); // ������Ʈ ����
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

    // ���� ���� �Է� �ޱ�
    cout << "Enter the number of stars: ";
    cin >> newConstellation.numStars;

    if (newConstellation.numStars < 2) {
        cout << "Wrong input. Please enter a value greater than 1." << endl;
        return;
    }

    // �� ���� ��ǥ �Է� �ޱ�
    cout << "Enter the position of the stars: " << endl;
    for (int i = 0; i < newConstellation.numStars; i++) {
        float x, y;
        cin >> x >> y;
        newConstellation.stars.push_back({ x, y });
    }

    // ��� �� ��ǥ �� ���� �Ÿ� ���
    vector<pair<float, pair<int, int>>> distances; // �Ÿ��� �ش� �� ��ǥ ���� �����ϱ� ���� ����
    for (int i = 0; i < newConstellation.numStars - 1; i++) { // ��� ���� ��ǥ �ֿ� ����
        for (int j = i + 1; j < newConstellation.numStars; j++) { // i ���� ������ ������ ��� ������ �Ÿ� ���
            float dx = newConstellation.stars[i].x - newConstellation.stars[j].x;
            float dy = newConstellation.stars[i].y - newConstellation.stars[j].y;
            float distance = sqrt(dx * dx + dy * dy); // �Ÿ� ���
            distances.push_back({ distance, {i, j} }); // �Ÿ��� �ش� �� ��ǥ ���� distances ���Ϳ� �߰�
            newConstellation.costs.push_back(distance); // �Ÿ��� costs ���Ϳ� �߰�
        }
    }

    // �Ÿ� �������� ����
    sort(distances.begin(), distances.end());

    // ũ�罺Į �˰������� �ּ� ��� ���� Ʈ�� ����
    newConstellation.parent.resize(newConstellation.numStars); // parent �迭 ũ�� ����
    newConstellation.rank.resize(newConstellation.numStars); // rank �迭 ũ�� ����
    for (int i = 0; i < newConstellation.numStars; i++) { // ��� ���� ���� �ݺ�
        makeSet(newConstellation, i); // �� ���� ���� �ʱ� ���� ����
    }

    int numEdges = 0; // ������� �߰��� ������ ��
    int idx = 0; // distances �ε���
    while (numEdges < newConstellation.numStars - 1) { // ���� Ʈ���� �ϼ��� ������ �ݺ�
        int u = distances[idx].second.first;
        int v = distances[idx].second.second;
        int uRoot = findSet(newConstellation, u); // ù ��° ������ ��Ʈ ��� ã��
        int vRoot = findSet(newConstellation, v); // �� ��° ������ ��Ʈ ��� ã��

        if (uRoot != vRoot) { // �� ������ �ٸ� ���տ� ���� �ִٸ�
            newConstellation.edges.push_back({ u, v }); // ���� �߰�
            unionSets(newConstellation, uRoot, vRoot); // �� ������ ��ħ
            numEdges++; // �߰��� ������ �� ����
            newConstellation.minCost += distances[idx].first; // �ּ� ��� ������Ʈ
        }
        idx++;
    }

    // ������ ���ڸ� �߰�
    constellations.push_back(newConstellation);
    cout << "New constellation has been created!" << endl;
}

//--------------------------------------------------------------
void ofApp::resetConstellations() {
    constellations.clear(); // ���ڸ� �ʱ�ȭ
}

//--------------------------------------------------------------
void ofApp::makeSet(Constellation& constellation, int v) {
    constellation.parent[v] = v; // v�� ��ü���� �������� ����
    constellation.rank[v] = 0; // �ʱ� ��ũ�� 0���� ����
}

//--------------------------------------------------------------
int ofApp::findSet(Constellation& constellation, int v) {
    if (v == constellation.parent[v]) { // v�� ������ ��Ʈ�� ���
        return v; // v�� ��ȯ
    }
    // v�� �θ� ��������� ã�ư���
    return constellation.parent[v] = findSet(constellation, constellation.parent[v]);
}

//--------------------------------------------------------------
void ofApp::unionSets(Constellation& constellation, int a, int b) {
    if (constellation.rank[a] < constellation.rank[b]) { // a�� ��ũ�� b�� ��ũ���� ���� ���
        constellation.parent[a] = b; // a�� �θ� b�� ����
    }
    else if (constellation.rank[a] > constellation.rank[b]) { // a�� ��ũ�� b�� ��ũ���� Ŭ ���
        constellation.parent[b] = a; //b�� �θ� a�� ����
    }
    else { // a�� ��ũ�� b�� ��ũ�� ���� ���
        constellation.parent[b] = a; // b�� �θ� a�� ����
        constellation.rank[a]++; // a�� ��ũ ����
    }
}

//--------------------------------------------------------------
void ofApp::drawStar(float x, float y, float radius, int numPoints) {
    float angle = TWO_PI / numPoints; // �� ���� ����
    float halfAngle = angle / 2.0; // ������ ����

    ofBeginShape();
    for (int i = 0; i < numPoints * 2; i++) { // ���� �� ���� ���� �ݺ�
        float r = radius * ((i % 2 == 0) ? 1.0 : 0.5); // �������� ������ ũ�⸦ �����ư��鼭 ����
        float currAngle = i * angle - halfAngle; // ���� ���� ���� ���
        float px = x + cos(currAngle) * r; // ���� ���� x ��ǥ ���
        float py = y + sin(currAngle) * r; // ���� ���� y ��ǥ ���
        ofVertex(px, py); // ���� ��ǥ�� �� �׸���
    }
    ofEndShape();
}