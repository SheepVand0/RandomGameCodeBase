// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_PrimaryTextBlock.h"
#include "Engine/Font.h"

UUI_PrimaryTextBlock::UUI_PrimaryTextBlock() {
	ConstructorHelpers::FObjectFinder<UFont> l_Font(TEXT("/Script/Engine.Font'/GameCore/Fonts/Oswald/F_Oswald.F_Oswald'"));
	Font.FontObject = l_Font.Object;
	Font.TypefaceFontName = FName("Oswald Regular");
}