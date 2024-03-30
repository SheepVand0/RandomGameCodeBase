// Fill out your copyright notice in the Description page of Project Settings.


#include "UI_SecondaryTextBlock.h"
#include "Engine/Font.h"

UUI_SecondaryTextBlock::UUI_SecondaryTextBlock() {
	ConstructorHelpers::FObjectFinder<UFont> l_Font(TEXT("/Script/Engine.Font'/GameCore/Fonts/Oswald/F_Oswald.F_Oswald'"));
	Font.FontObject = l_Font.Object;
	Font.TypefaceFontName = FName("Oswald Light");
}