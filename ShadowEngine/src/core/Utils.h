#pragma once
#include <vector>
#include <map>
#include "Vector.h"
#include <glm/ext/matrix_transform.hpp>

template<typename T>
static bool containsElement(const std::vector<T> vector, const T element)
{
	return std::find(vector.begin(), vector.end(), element) != vector.end();
}

static mat4 createModel(vec3 pos, vec3 scale, vec3 rotation)
{
	glm::mat4 model(1.0f);
	model = glm::translate(model, pos);

	model = glm::rotate(model, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	model = glm::rotate(model, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	model = glm::scale(model, scale);

	return model;
}

template<typename key, typename value>
using simplemap = std::map<key, value>;

/*
	Tests if a key is in the map. If it is it returns the value and
	else it will use given function and arguments to create the value,
	put it in the map and return it. Just ignore the templates.
	@param map: The map that should be looked in and modified. Use std::map.
	@param key: The key that should be searched.
	@param f: The function to create the value.
	@param a: The arguments for f.
*/
template<typename MV, typename MK, typename F, typename... A>
static MV getMappedValue(simplemap<MK, MV>& map, MK key, F f, A... a)
{
	auto searchResult = map.find(key);
	if (searchResult == map.end())
	{
		MV result = f(a...);
		map.insert(std::pair(key, result));
		return result;
	}
	return searchResult->second;
}