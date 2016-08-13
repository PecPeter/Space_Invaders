#ifndef HANDLER_HPP
#define HANDLER_HPP

#include "2D-Engine/stateHandler.hpp"

#include "gameStateTypes.hpp"

#include "mainState.hpp"

class cHandler : public cStateHandler
{
	public:
		cHandler (void);
		~cHandler (void);

		cGameState* getState (void);
		void changeState (int stateAction);

	private:
		cGameState* state_;
};

#endif
