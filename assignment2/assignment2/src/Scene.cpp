#include "Scene.hpp"

void Scene::addChild(const Object& child) {
	mChildren.push_back(child);
}

void Scene::removeChild(const std::vector<Object>::iterator& titerator) {
	mChildren.erase(titerator);
}

Object& Scene::popChild() {
	auto& temp = mChildren.back();

	mChildren.pop_back();

	return temp;
}

void Scene::draw() {
	for (auto& child: mChildren) 
		child.draw();
}

void Scene::update(const int deltaTime, const int elapsedTime) {
	for (auto& child : mChildren)
		child.update(deltaTime, elapsedTime);
}

void Scene::addAudio(const Audio& audio) {
	mAudios.push_back(audio);
}

bool Scene::isAudioPlaying(const int i) const {
	return !mAudios[i].paused;
}

void Scene::playAudio(const int i) const {
	mAudios[i].start();
}

void Scene::resumeAudio(const int i) {
	mAudios[i].resume();
}

void Scene::stopAudio(const int i) const {
	mAudios[i].stop();
}

