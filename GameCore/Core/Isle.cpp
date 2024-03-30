// Fill out your copyright notice in the Description page of Project Settings.


#include "Isle.h"

// Sets default values
AIsle::AIsle()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	EventComponent = CreateDefaultSubobject<UEventComponent>(TEXT("Event Component"));

	DataComponent = CreateDefaultSubobject<UDataComponent>(TEXT("Data Component"));
}

// Called when the game starts or when spawned
void AIsle::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(TEXT("Isle"));
}

// Called every frame
void AIsle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


}

void AIsle::OnEventReceived(FString p_Event) {
	if (p_Event == RESET_PLAYERS_EVENT) {
		SpawnPlayers(DataComponent->GetFloat(LAST_WINNER));
	}
}

void AIsle::SpawnPlayers(int p_Winner) {

	TMap<int, int> l_SpawnedPlayersForTeam;

	TArray<AC_Player*> l_Players;
	TArray<AActor*> l_PrePlayers;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TAG_PLAYER, l_PrePlayers);
	for (int l_i = 0; l_i < l_PrePlayers.Num(); l_i++) {
		l_Players.Add(Cast<AC_Player>(l_PrePlayers[l_i]));
	}

	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TAG_BOT, l_PrePlayers);
	for (int l_i = 0; l_i < l_PrePlayers.Num(); l_i++) {
		l_Players.Add(Cast<AC_Player>(l_PrePlayers[l_i]));
	}

	FVector l_RightVector = FVector(1, 0, 0);

	for (int l_i = 0; l_i < l_Players.Num(); l_i++) {
		auto l_Team = l_Players[l_i]->TeamIndex;
		FVector l_AverageSpawnPosition = Spawns[l_Team];
		l_Players[l_i]->SetActorLocation(l_RightVector * (l_SpawnedPlayersForTeam[l_Team] * 2));
		l_Players[l_i]->ResetPlayer(l_Team != p_Winner, l_Team != p_Winner);
		l_Players[l_i]->GetRootComponent()->SetVisibility(true, true);
	}
}