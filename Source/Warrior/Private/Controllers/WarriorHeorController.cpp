// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorHeorController.h"

AWarriorHeorController::AWarriorHeorController()
{
    HeroTeamId = FGenericTeamId(0);
}

FGenericTeamId AWarriorHeorController::GetGenericTeamId() const
{
    return HeroTeamId;
}
