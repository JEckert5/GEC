#include <cstdio>
#include <Windows.h>
#include <box2d/b2_world.h>
#include <box2d/b2_body.h>
#include <box2d/b2_polygon_shape.h>
#include <box2d/b2_chain_shape.h>
#include <box2d/b2_fixture.h>

int main() {
	b2PolygonShape square;
	square.SetAsBox(0.5f, 0.5f, {0, 0}, 0);

	b2Vec2 vs[4];
	vs[0].Set(-5, 5);
	vs[1].Set(5, 5);
	vs[2].Set(5, -5);
	vs[3].Set(-5, -5);

	b2ChainShape border;
	border.CreateLoop(vs, 4);
}