#include "Puzzle.h"
#include "EscapeAutomate.hpp"

void Puzzle::RegisterProperty(BaseProperty* prop)
{
	prop->ParentPuzzleId = this->PuzzleObject->PuzzleId;
	prop->PropertyId = Properties.size() + 1;
	Properties.insert(std::pair<uint16_t, BaseProperty*>(prop->PropertyId, prop));
}

void Puzzle::UpdatePuzzleStatus(PuzzleStatus status)
{
	PuzzleObject->Status = status;
	EscapeAutomate.SendPuzzlePropertyChanged(PuzzleObject->PuzzleId, 0, "Status", String(status));
}

void Puzzle::InnerLoop(void* puzzle)
{
	Puzzle* puz = (Puzzle*)puzzle;
	while (true)
	{
		puz->Loop();
	}
}
