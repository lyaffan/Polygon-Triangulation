#pragma once
#include <GL/glew.h>
#include <vector>
#include <queue>
#include <iostream>
#include <string>
#include <stack>

#include "MainProgram.h"
#include "DCEL.h"
#include "BinarySearchTree.h"

class MainProgram;

class Event {
public:
	EventType type;
	Vertex *vertex;

	Event(EventType type, Vertex *vertex) {
		this->type = type;
		this->vertex = vertex;
	}
};

struct EventComparator {
	bool operator()(const Event& event1, const Event& event2) {
		if (event1.vertex->coordinates->y != event2.vertex->coordinates->y) {
			return event1.vertex->coordinates->y < event2.vertex->coordinates->y;
		}
		else {
			return event1.vertex->coordinates->x > event2.vertex->coordinates->x;
		}
	}
};

class Status {
public:
	Edge *edge;
	Vertex *helper;
	
	Status(Edge *edge, Vertex *helper) {
		this->edge = edge;
		this->helper = helper;
	}
	bool operator > (const Status &status2) {
		return edge->originVertex->coordinates->x > status2.edge->originVertex->coordinates->x;
	}
};

class TurnVertex {
public:
	Vertex *vertex;
	TurnVertex *next;
	TurnVertex *previous;

	TurnVertex(Vertex *vertex, TurnVertex *next, TurnVertex* previous) {
		this->vertex = vertex;
		this->next = next;
		this->previous = previous;
	}
};

class PolygonTriangulation {
public:
	PolygonTriangulation(MainProgram *mainProgram);
	~PolygonTriangulation();

	void draw();
	void initPolygon();
	void initVertexTypes();
	void makeMonotone(Polygon *polygon);
	void triangulateMonotone();

private:
	MainProgram *mainProgram;
	Polygon *polygon;
	priority_queue<Event, std::vector<Event>, EventComparator> events;
	vector<Status*> status;
	vector<TurnVertex*> turnVertices;
	vector<Vertex**> verticesToSplit;
	vector<Vertex**> verticesToSplit2;

	Vertex *debugVertex;
	Edge *debugEdge;

	void splitFace(Vertex *vertexFrom, Vertex *vertexTo);
	Status* searchDirectlyLeftStatus(Vertex *vertex);
	EventType determineVertexType(Vertex &vertex, Vertex &previous, Vertex &next);
	void checkVertexHandler(Event &event);
	void handleStartVertex(Vertex *vertex);
	void handleEndVertex(Vertex *vertex);
	void handleSplitVertex(Vertex *vertex);
	void handleMergeVertex(Vertex *vertex);
	void handleRegularVertex(Vertex *vertex);
	int vertexPositionToEdge(Vertex *vertex, Edge *edge);

	void initVertexDirections(vector<Edge*> &sortedEdges);
	float distance(Coordinates *a, Coordinates *b);
	bool checkRange(Coordinates *p, Coordinates *p1, Coordinates *p2);
	bool isIntersecting(Coordinates *a1, Coordinates *a2, Coordinates *b1, Coordinates *b2);
	float calculateAngle(Coordinates *a, Coordinates *b, Coordinates *c);
	bool isInsidePolygon(Face *face, Edge *e1, Edge *e2);
};