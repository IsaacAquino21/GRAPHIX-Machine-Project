#pragma once

#include "Model.h"

class Player {
private:
	Model model;

public:
	Player(Model m): model(m) {

	}

	Model getModel() {
		return model;
	}
};