// Fill out your copyright notice in the Description page of Project Settings.


#include "SPlayerState.h"

int32 ASPlayerState::GetCredits() const
{
	return credits;
}

void ASPlayerState::AddCredits(int32 delta)
{
	if (!ensure(delta > 0.0f)) return;

	credits += delta;

	onCreditsChanged.Broadcast(this, credits, delta);
}

bool ASPlayerState::RemoveCredits(int32 Delta)
{
	if (!ensure(Delta > 0.0f)) return false;

	if (credits < Delta)
	{
		// Not enough credits available
		return false;
	}

	credits -= Delta;

	onCreditsChanged.Broadcast(this, credits, -Delta);
	
	return true;
}

