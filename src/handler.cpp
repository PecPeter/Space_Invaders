#include "handler.hpp"

cHandler::cHandler (void): state_(nullptr)
{
	state_ = new cMainState;
	numStates_ = 1;
}

cHandler::~cHandler (void)
{
	if (state_ != nullptr)
		delete state_;
}

cGameState* cHandler::getState (void)
{
	return state_;
}

void cHandler::changeState (int stateAction)
{
	switch (stateAction)
	{
		case eStateAction::NONE:
			break;
		case eStateAction::REMOVE_STATE:
			delete state_;
			state_ = nullptr;
			--numStates_;
			break;
		case eStateAction::MENU_STATE:
			break;
		case eStateAction::MAIN_STATE:
			break;
		default:
			break;
	}
}
