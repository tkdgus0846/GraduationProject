// Fill out your copyright notice in the Description page of Project Settings.


#include "RootWidget.h"

void URootWidget::Setup()
{
	this->AddToViewport();
}

void URootWidget::TearDown()
{
	this->RemoveFromViewport();
}
