#include "utils.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include "World.hpp"

pair<int, int> operator- (const pair<int, int>& l, const pair<int, int>& r) {
	return {l.first - r.first, l.second - r.second};
}

pair<int, int> operator+ (const pair<int, int>& l, const pair<int, int>& r) {
	return {l.first + r.first, l.second + r.second};
}

vector<Vector2f> square(float x, float y, float w, float h) {
	return {
		{x, y}, 
		{x + w, y}, 
		{x + w, y + h}, 
		{x, y + h}
	};
}


void arrowChange(RenderWindow* window, vector<SDL_Scancode>& keys, bool* direction, void (*foo)(vector<void*>), vector<void*> passingArgument) {
	bool unpressed = true;
	for (SDL_Scancode key : keys) {
		if (window->keyboard[key]) {
			if (!(*direction)) {
				if (foo != nullptr) {
					foo(passingArgument);
				}
				*direction = true;
			}
			unpressed = false;
		}
	}
	if (unpressed) {
		*direction = false;
	}
}

float pointAngleBetween(int x1, int y1, int x2, int y2) {
	float angle;
	float xDiff = x1 - x2;
	float yDiff = y1 - y2;
	if (xDiff == 0) {
		if (yDiff >= 0) {
			angle = M_PI / -2.0;
		}
		else {
			angle = M_PI / 2.0;	
		}
	}
	else if (xDiff > 0) {
		angle = -atan(yDiff / xDiff);
	}
	else {
		angle = M_PI - atan(yDiff / xDiff);
	}
	return angle;
}

float dotProduct(Vector2f vec1, Vector2f vec2) {
	return vec1.x * vec2.x + vec1.y * vec2.y;	
}

Vector2f vectorFromAngle(float angle) {
	Vector2f vec;
	vec.x = cos(angle);
	vec.y = -sin(angle);
	return vec;	
}

Vector2f getMinMax(Vector2f vec, const vector<Vector2f>& points) {
	// vec.y *= -1;
	Vector2f answer;
	float point = dotProduct(vec, points[0]);
	answer.x = point;
	answer.y = point;
	for (int x = 1; x < points.size(); x++) {
		point = dotProduct(vec, points[x]);
		answer.x = min(answer.x, point);
		answer.y = max(answer.y, point);
	}
	return answer;
}

void addUnique(Vector2f vec, vector<float>& anglesToCheck) {
	if (vec.x != 0) {
		float angle = M_PI / 2 - atan(vec.y / vec.x);
		if (find(anglesToCheck.begin(), anglesToCheck.end(), angle) == anglesToCheck.end()) {
			anglesToCheck.push_back(angle);
		}		
	}
}

bool collides(RenderWindow* window, vector<Vector2f> polygonPoints, vector<Vector2f> playerPoints) {
	bool allPointsOut = true;
	for (int x = 0; x < polygonPoints.size(); x++) {
		if (window->x < polygonPoints[x].x && window->x + RenderWindow::WIDTH > polygonPoints[x].x && window->y < polygonPoints[x].y && window->y + RenderWindow::HEIGHT > polygonPoints[x].y) {
			allPointsOut = false;
		}
	}
	if (allPointsOut) {
		return false;
	}

	vector<float> anglesToCheck;

	for (int x = 0; x < playerPoints.size() - 1; x++) {
		addUnique(playerPoints[x] - playerPoints[x + 1], anglesToCheck);
	}
	addUnique(playerPoints[0] - playerPoints[playerPoints.size() - 1], anglesToCheck);

	for (int x = 0; x < polygonPoints.size() - 1; x++) {
		addUnique(polygonPoints[x] - polygonPoints[x + 1], anglesToCheck);
	}
	addUnique(polygonPoints[0] - polygonPoints[polygonPoints.size() - 1], anglesToCheck);

	for (int y = 0; y < anglesToCheck.size(); y++) {
		float angle = anglesToCheck[y];
		Vector2f vec = vectorFromAngle(angle);

		Vector2f polygonMM = getMinMax(vec, polygonPoints);
		Vector2f playerMM = getMinMax(vec, playerPoints);

		float length = min(polygonMM.y - playerMM.x, playerMM.y - polygonMM.x);
		if (length < 0) {
			return false;
		}
	}
	return true;
}

void fixPreLoop(RenderWindow* window, bool& extendXP, bool& extendXN, bool& extendYP, bool& extendYN) {
	extendXP = window->x + RenderWindow::WIDTH / window->zoom > World::WORLDLENGTH;
	extendXN = window->x < 0;
	extendYP = window->y + RenderWindow::HEIGHT / window->zoom > World::WORLDLENGTH;
	extendYN = window->y < 0;
}

void loopPreFix(Entity* go, bool extendXP, bool extendXN, bool extendYP, bool extendYN) {
	go->x = (extendXP && go->x < World::WORLDLENGTH / 2) ? go->x + World::WORLDLENGTH : go->x;
	go->x = (extendXN && go->x > World::WORLDLENGTH / 2) ? go->x - World::WORLDLENGTH : go->x;
	go->y = (extendYP && go->y < World::WORLDLENGTH / 2) ? go->y + World::WORLDLENGTH : go->y;
	go->y = (extendYN && go->y > World::WORLDLENGTH / 2) ? go->y - World::WORLDLENGTH : go->y;
}

void loopPostFix(Entity* go) {
	go->x -= int(floor(go->x / World::WORLDLENGTH)) * World::WORLDLENGTH;
	go->y -= int(floor(go->y / World::WORLDLENGTH)) * World::WORLDLENGTH;
}