// Fill out your copyright notice in the Description page of Project Settings.

#include "C_Player.h"
#include "MotionControllerComponent.h"
#include "Engine.h"
#include "Kismet/KismetSystemLibrary.h"
#include "IXRTrackingSystem.h"
#include "IHeadMountedDisplay.h"
#include "Internationalization/Text.h"
#include "GameCore/Public/GameCore.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AC_Player::AC_Player()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//-----------------------Asset finding-----------------------
	ConstructorHelpers::FObjectFinder<USkeletalMesh> l_HandMesh(TEXT("SkeletalMesh'/GameCore/SKMeshes/PlayerHands/PlayerHand.PlayerHand'"));
	ConstructorHelpers::FObjectFinder<UStaticMesh> l_Cube(TEXT("/Script/Engine.StaticMesh'/Game/Assets/Meshes/Cube.Cube'"));
	ConstructorHelpers::FObjectFinder<UMaterialInstance> l_CubeMeshMaterial(TEXT("/Script/Engine.MaterialInstanceConstant'/GameCore/Materials/Instances/M_CubePlayerBody.M_CubePlayerBody'"));
	ConstructorHelpers::FObjectFinder<USkeletalMesh> l_PlayerSKMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ControlRig/Characters/Mannequins/Meshes/SKM_Manny.SKM_Manny'"));
	ConstructorHelpers::FObjectFinder<UAnimInstance> l_BasePlayerAnimations(TEXT("/Script/Engine.AnimBlueprint'/Game/ControlRig/Characters/Mannequins/Animations/ABP_Manny.ABP_Manny'"));

	//-----------------------Components Setups-----------------------
	FpsCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("Fps camera"));
	FpsCamera->SetupAttachment(GetRootComponent());
	FpsCamera->bUsePawnControlRotation = true;

	RightVrHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right VR Hand"));
	RightVrHand->SetupAttachment(GetRootComponent());
	RightVrHand->SetTrackingSource(EControllerHand::Right);

	LeftVrHand = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left VR Hand"));
	LeftVrHand->SetupAttachment(GetRootComponent());
	LeftVrHand->SetTrackingSource(EControllerHand::Left);

	FVector l_HandScale = FVector(0.15f, 0.15f, 0.15f);

	RightHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Right Hand"));
	RightHand->SetupAttachment(FpsCamera);
	RightHand->SetSkeletalMesh(l_HandMesh.Object);
	RightHand->SetRelativeScale3D(l_HandScale);
	FQuat l_HandRotation = FQuat(FRotator(0, 90, 0));
	RightHand->SetRelativeRotation(l_HandRotation);

	HandInventoryItem = CreateDefaultSubobject<USceneComponent>(TEXT("Item Hand"));
	HandInventoryItem->SetupAttachment(RightHand);
	HandInventoryItem->SetRelativeRotation(FQuat::MakeFromEuler(FVector(180, 90, 0)));
	HandInventoryItem->SetRelativeLocation(FVector3d(29, -84, 10));

	LeftHand = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Left Hand"));
	LeftHand->SetupAttachment(FpsCamera);
	LeftHand->SetSkeletalMesh(l_HandMesh.Object);
	LeftHand->SetRelativeScale3D(l_HandScale);
	LeftHand->SetRelativeRotation(l_HandRotation);

	RightHand->SetRelativeLocation(FVector(25, 30, -25));
	LeftHand->SetRelativeLocation(FVector(25, -30, -25));

	PlayerMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Head"));
	PlayerMesh->bOwnerNoSee = true;
	//PlayerMesh->SetStaticMesh(l_Cube.Object);
	PlayerMesh->SetSkeletalMesh(l_PlayerSKMesh.Object);
	PlayerMesh->SetupAttachment(FpsCamera);
	PlayerMesh->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	//PlayerMesh->SetAnimInstanceClass(l_BasePlayerAnimations.Object->StaticClass());
	//PlayerMesh->ComponentTags.Add(FName("HeadComponent"));
	//PlayerMesh->SetMaterial(0, l_CubeMeshMaterial.Object);

	PawnSensing = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensing"));
	PawnSensing->SightRadius = 1500.0f;
	PawnSensing->OnSeePawn.AddDynamic(this, &AC_Player::OnBotSawPawn);

	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("Health Component"));
	HealthComponent->SetBaseLife(100);
	HealthComponent->SetBaseShield(75);
	HealthComponent->SetLife(100, 75);
	HealthComponent->OnLifeChange.AddDynamic(this, &AC_Player::OnLifeChange);
	HealthComponent->OnDeath.AddDynamic(this, &AC_Player::KillPlayer);

	DataComponent = CreateDefaultSubobject<UDataComponent>(TEXT("Data Component"));
	DataComponent->AddObject(FString("FPSCamera"), FpsCamera);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>("Inventory Component");
	TMap<int, FString> l_InventoryCategories;
	l_InventoryCategories.Add(0, BODY_WEAPONS_CATEGORY);
	l_InventoryCategories.Add(1, PISTOLS_CATEGORY);
	l_InventoryCategories.Add(2, MAIN_WEAPONS_CATEGORY);
	l_InventoryCategories.Add(3, SNIPERS_CATEGORY);

	InventoryComponent->SetInventoryLength(4, l_InventoryCategories);
	InventoryComponent->OnItemSelected.AddDynamic(this, &AC_Player::OnItemSelected);
	InventoryComponent->OnItemAdded.AddDynamic(this, &AC_Player::OnItemAdded);
	InventoryComponent->OnNeedUpdateOnItemRemoved.AddDynamic(this, &AC_Player::OnItemRemovedAndNeedChange);
	//InventoryComponent->AddInventoryItem(FItemProperties::CreateItemProperties(AFlashInitializer::StaticClass(), FString("Initializer"), FTransform(), false, nullptr));

	AbilityComponent = CreateDefaultSubobject<UAbilityComponent>(TEXT("Ability Component"));
	FAbilityModel l_AbilityModel = FAbilityModel();
	l_AbilityModel.Init(0, 2, 1, true, AFlashInitializer::StaticClass(), AFlashAbility::StaticClass(), true, 1.0f, FString());
	AbilityComponent->SetAbility(TArray<FAbilityModel>{ l_AbilityModel });

	AIControllerClass = AC_BotController::StaticClass();

	//-----------------------Defaults variables-----------------------

	Energy = 1.0f;
	MaxEnergyLevel = 32.0f;
	MinEnergyLevel = 1.0f;

	m_DefaultSpeed = 750.0f;
	m_SprintMultiplier = 1;
	m_IsSprinting = false;

	HandsAndCameraHeight = 50;
	CapsuleHeight = 20.0f;

	m_HeadTransform.SetLocation(FVector(0, 0, 0));
	m_HeadTransform.SetScale3D(FVector(0.5f, 0.5f, 0.5f));
	m_HeadTransform.SetRotation(FQuat(FRotator(0, 0, 0)));

	m_BodyTransform.SetLocation(FVector(-80.0f, 0, 0/*-48.8f*/));
	m_BodyTransform.SetScale3D(FVector(1.5f, 1, 0.7f));
	m_BodyTransform.SetRotation(FQuat(FRotator(0, 0, 0)));

	EventComponent = CreateDefaultSubobject<UEventComponent>(TEXT("Event Component"));
	EventComponent->OnEventSent.AddDynamic(this, &AC_Player::OnEventReceived);

	AttackingPawn = nullptr;
}

void AC_Player::OnConstruction(const FTransform& Transform)
{
	FpsCamera->SetRelativeLocation(FVector(0, 0, HandsAndCameraHeight));

	PlayerMesh->SetRelativeTransform(m_HeadTransform);

	GetCapsuleComponent()->SetCapsuleHalfHeight(CapsuleHeight, false);
}

// Called when the game starts or when spawned
void AC_Player::BeginPlay()
{
	Super::BeginPlay();

	check(GEngine);

	InventoryComponent->AddInventoryItem(FGameCore::GetHandProperties());
	InventoryComponent->SelectInventoryItem(0);

	if (Cast<APlayerController>(GetController()) != nullptr) {
		OnBlueprintSetupEvent.Broadcast();
		Tags.Add(TAG_PLAYER);
	}
	else {
		Tags.Add(TAG_BOT);
	}

	if (GEngine->XRSystem != nullptr) {
		auto l_Device = GEngine->XRSystem->GetHMDDevice();
		if (l_Device != nullptr) {
			if (l_Device->IsHMDConnected())
				l_Device->EnableHMD(true);
		}
	}

	UpdateMainPlayerWidget();
}

// Called every frame
void AC_Player::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AC_Player::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->AddActionBinding(FInputActionBinding());

	PlayerInputComponent->BindAxis("MoveForward", this, &AC_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveBackward", this, &AC_Player::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AC_Player::MoveRight);
	PlayerInputComponent->BindAxis("MoveLeft", this, &AC_Player::MoveRight);
	PlayerInputComponent->BindAxis("LookX", this, &AC_Player::LookX);
	PlayerInputComponent->BindAxis("LookY", this, &AC_Player::LookY);

	PlayerInputComponent->BindAction("ScrollUp", IE_Pressed, this, &AC_Player::GoToUpperItem);
	PlayerInputComponent->BindAction("ScrollDown", IE_Pressed, this, &AC_Player::GoToLowerItem);

	PlayerInputComponent->BindAction("Trigger", IE_Pressed, this, &AC_Player::UseItem);
	PlayerInputComponent->BindAction("Trigger", IE_Released, this, &AC_Player::StopUseItem);
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AC_Player::PickupItemOnScene);
	PlayerInputComponent->BindAction("DropItem", IE_Pressed, this, &AC_Player::DropCurrentItem);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AC_Player::NotifyReload);

	PlayerInputComponent->BindAction("InvItem0", IE_Pressed, this, &AC_Player::InvItem0);
	PlayerInputComponent->BindAction("InvItem1", IE_Pressed, this, &AC_Player::InvItem1);
	PlayerInputComponent->BindAction("InvItem2", IE_Pressed, this, &AC_Player::InvItem2);
	PlayerInputComponent->BindAction("InvItem3", IE_Pressed, this, &AC_Player::InvItem3);
	//PlayerInputComponent->BindAction("InvItem4", IE_Pressed, this, &AC_Player::InvItem4);
	//PlayerInputComponent->BindAction("InvItem5", IE_Pressed, this, &AC_Player::InvItem5);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &AC_Player::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &AC_Player::StopJumping);

	//PlayerInputComponent->BindAction("Ability1", IE_Pressed, this, &AC_Player::Ability1);

	m_PlayerInputComponent = PlayerInputComponent;
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

AGameManager* AC_Player::GetGameManager() {
	AGameManager* l_Manager = nullptr;

	l_Manager = Cast<AGameManager>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameManager::StaticClass()));
	return l_Manager;
}

void AC_Player::ResetPlayer(bool p_ResetInventory, bool p_KeepShield) {
	if (p_ResetInventory) {
		InventoryComponent->SetInventoryItem(1, FItemProperties::GetNullItem());
		InventoryComponent->SetInventoryItem(2, FItemProperties::GetNullItem());
		InventoryComponent->SetInventoryItem(3, FItemProperties::GetNullItem());
	}
	HealthComponent->SetLife(100, (p_KeepShield) ? HealthComponent->GetShield() : 0);
	ResetEnergy();
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Player::SetWidgetReference(UUI_Player* p_Widget) {
	m_PlayerWidget = p_Widget;
}

void AC_Player::UpdateMainPlayerWidget() {
	m_PlayerWidget->AddToPlayerScreen();
	m_PlayerWidget->SetLife(HealthComponent->GetLife());
	m_PlayerWidget->SetShield(HealthComponent->GetShield());
	m_PlayerWidget->StartTimer(120);
	m_PlayerWidget->HideAmmoBox();
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Player::InvItem0() { InventoryComponent->SelectInventoryItem(0); }
void AC_Player::InvItem1() { InventoryComponent->SelectInventoryItem(1); }
void AC_Player::InvItem2() { InventoryComponent->SelectInventoryItem(2); }
void AC_Player::InvItem3() { InventoryComponent->SelectInventoryItem(3); }

//void AC_Player::Ability1() { AbilityComponent->UseAbility(0, InventoryComponent); }

void AC_Player::OnItemAdded(FItemProperties p_Item, int p_Index) {

}

void AC_Player::OnItemSelected(FItemProperties p_Item) {
	if (HandItem != nullptr)
	{
		HandItem->UnSelect();
		HandItem->Destroy();
	}

	if (m_PlayerWidget != nullptr)
		m_PlayerWidget->HideAmmoBox();

	HandItem = GetWorld()->SpawnActor<AC_Item>(p_Item.ItemClass);
	if (HandItem == nullptr) return;
	HandItem->Select(this, HandInventoryItem);
	FTransform l_HandedTransform = HandItem->GetPropertiesCopy().HandedTransform;
	l_HandedTransform.SetScale3D(l_HandedTransform.GetScale3D() * 4);
	HandItem->SetActorRelativeTransform(l_HandedTransform);
}

void AC_Player::OnItemRemovedAndNeedChange() {
	InventoryComponent->SelectInventoryItem(0);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Player::KillPlayer()
{
	APlayerController* l_Controller = Cast<APlayerController>(GetController());
	if (l_Controller != nullptr) {
		FActorSpawnParameters l_Parameters;
		l_Parameters.bNoFail = true;
		l_Parameters.bAllowDuringConstructionScript = false;
		l_Parameters.Name = FName("Default");
		ASpectatorPawn* l_Pawn = GetWorld()->SpawnActor<ASpectatorPawn>(GetActorLocation(), GetActorRotation(), l_Parameters);
		l_Controller->Possess(l_Pawn);
	}
	GetRootComponent()->SetVisibility(false, true);
}

void AC_Player::OnLifeChange(int p_Life, int p_Shield) {
	m_PlayerWidget->SetLife(p_Life);
	m_PlayerWidget->SetShield(p_Shield);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Player::LookX(float p_value)
{
	if (p_value != 0)
		AddControllerYawInput(p_value);
}

void AC_Player::LookY(float p_value)
{
	if (p_value != 0)
		AddControllerPitchInput(p_value);
}

void AC_Player::MoveForward(float p_value)
{
	if (p_value != 0.0f)
		AddMovementInput(GetActorForwardVector(), (m_IsSprinting) ? p_value * m_SprintMultiplier : p_value, false);
}

void AC_Player::MoveRight(float p_value)
{
	if (p_value != 0.0f)
		AddMovementInput(GetActorRightVector(), (m_IsSprinting) ? p_value * m_SprintMultiplier : p_value, false);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Player::UseItem()
{
	IsTriggerPressed = true;

	if (HandItem == nullptr)
		return;

	HandItem->Use();
}

void AC_Player::StopUseItem()
{
	IsTriggerPressed = false;

	if (HandItem == nullptr)
		return;

	HandItem->StopUse();
}

void AC_Player::NotifyReload() {
	if (HandItem == nullptr) return;

	HandItem->NotifyEvent(RELOAD_EVENT);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Player::AddEnergyLevel(float p_level)
{
	Energy = Energy + p_level;
	if (Energy > MaxEnergyLevel)
	{
		Energy = MaxEnergyLevel;
	}
	UpdateEnergy();
}

void AC_Player::RemoveEnergyLevel(float p_level)
{
	Energy = Energy - p_level;
	if (Energy <= MinEnergyLevel)
	{
		Energy = MinEnergyLevel;
	}
	UpdateEnergy();
}

void AC_Player::UpdateEnergy()
{
	//FpsCamera->SetFieldOfView(FMath::Clamp(90 + (Energy * 0.15f), 80, 110));
	GetCharacterMovement()->MaxWalkSpeed = m_DefaultSpeed * Energy * 0.16f;
}

void AC_Player::ResetEnergy()
{
	GetCharacterMovement()->MaxWalkSpeed = m_DefaultSpeed;
}

void AC_Player::MultiplyEnergyByTwo()
{
	Energy = Energy * 2;
	if (Energy > MaxEnergyLevel)
	{
		Energy = MaxEnergyLevel;
	}
}

void AC_Player::DivideEnergyByTwo()
{
	if (Energy != 0)
	{
		Energy = Energy / 2;
	}
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Player::PlayerHitSomeone(AC_Player* p_player)
{
	float l_levelToAdd = 1 - (HealthComponent->GetLife() / HealthComponent->GetBaseLife()) * 0.17f;
	AddEnergyLevel(l_levelToAdd);
}

void AC_Player::PlayerHasBeenHit()
{
	if (HealthComponent->GetLife() < (int)(HealthComponent->GetBaseLife() * 0.25f))
	{
		float l_EnergyMultiplier = (1 - (HealthComponent->GetLife() / HealthComponent->GetBaseLife()) * 0.25f);

		AddEnergyLevel(l_EnergyMultiplier);
	}
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////

void AC_Player::PickupItemOnScene()
{
	FVector l_EndLocation = FpsCamera->GetForwardVector() * 500.0f + FpsCamera->GetComponentLocation();
	TArray<AActor*> l_ActorsToIgnore;
	l_ActorsToIgnore.Add(this);
	FHitResult l_OutHitResult;

	UKismetSystemLibrary::LineTraceSingle(
		this->GetWorld(),
		FpsCamera->GetComponentLocation(),
		l_EndLocation, ETraceTypeQuery::TraceTypeQuery1,
		true, l_ActorsToIgnore, EDrawDebugTrace::None,
		l_OutHitResult, true, FLinearColor::Green, FLinearColor::Red);

	AC_Item* l_TargetItem = Cast<AC_Item>(l_OutHitResult.GetActor());
	if (l_TargetItem != nullptr)
	{
		if (InventoryComponent->AddInventoryItem(l_TargetItem->GetPropertiesCopy()))
			l_TargetItem->Destroy();
	}
}

void AC_Player::DropCurrentItem()
{
	if (HandItem == nullptr)
		return;

	if (HandItem->GetPropertiesCopy().RemovableFromInventory == false) return;

	FItemProperties l_CurrentItemProperties = HandItem->GetPropertiesCopy();
	if (FItemProperties::Equal(l_CurrentItemProperties, FItemProperties::GetNullItem()))
		return;

	FActorSpawnParameters l_SpawnParams;
	l_SpawnParams.bNoFail = true;

	AC_Item* l_DropedItem = GetWorld()->SpawnActor<AC_Item>(GetActorLocation(), GetActorRotation(), l_SpawnParams);
	l_DropedItem->SetItemProperties(l_CurrentItemProperties);

	int l_CurrentItemIndex = InventoryComponent->TryGetInventoryItemIndex(l_CurrentItemProperties);
	if (l_CurrentItemIndex != -1)
		InventoryComponent->RemoveInventoryItem(l_CurrentItemIndex);
}

void AC_Player::GoToUpperItem()
{
	if (HandItem != nullptr)
	{
		// ReSharper disable once CppLocalVariableMayBeConst
		int l_CurrentItemIndex = InventoryComponent->TryGetInventoryItemIndex(HandItem->GetPropertiesCopy());
		if (l_CurrentItemIndex == -1)
			return;

		if (l_CurrentItemIndex == InventoryComponent->GetInventoryMaxLength())
			InventoryComponent->SelectInventoryItem(0);
		else
			InventoryComponent->SelectInventoryItem(l_CurrentItemIndex + 1);
	}
}

void AC_Player::GoToLowerItem()
{
	if (HandItem != nullptr)
	{
		// ReSharper disable once CppLocalVariableMayBeConst
		int l_CurrentItemIndex = InventoryComponent->TryGetInventoryItemIndex(HandItem->GetPropertiesCopy());
		if (l_CurrentItemIndex == -1)
			return;

		if (l_CurrentItemIndex == 0)
			InventoryComponent->SelectInventoryItem(InventoryComponent->GetInventoryMaxLength());
		else
			InventoryComponent->SelectInventoryItem(l_CurrentItemIndex - 1);
	}
}

void AC_Player::ShowWidgetAmmoBox() {
	if (m_PlayerWidget == nullptr) return;

	m_PlayerWidget->ShowAmmoBox();
}

void AC_Player::HideWidgetAmmoBox() {
	if (m_PlayerWidget == nullptr) return;

	m_PlayerWidget->HideAmmoBox();
}

void AC_Player::SetWidgetMagazineAmmo(int p_Value) {
	if (m_PlayerWidget == nullptr) return;

	m_PlayerWidget->SetMagazineAmmoCount(p_Value);
}

void AC_Player::SetWidgetMaxMagazineAmmo(int p_Value) {
	if (m_PlayerWidget == nullptr) return;

	m_PlayerWidget->SetMaxMagazineAmmoCount(p_Value);
}

void AC_Player::SetWidgetOtherAmmo(int p_Value) {
	if (m_PlayerWidget == nullptr) return;

	m_PlayerWidget->SetMaxAmmoCount(p_Value);
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
// Events

void AC_Player::OnPlayerDidKill() {
	Kills += 1;
	//Money += 400;
	if (PostKillAction != nullptr)
		(this->*PostKillAction)();
}

void AC_Player::OnEventReceived(FString p_Value) {
	if (p_Value == DID_KILL_EVENT)
		OnPlayerDidKill();
}

////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////
/// AI

bool AC_Player::IsBot() {
	return Cast<AC_BotController>(GetController()) != nullptr;
}

AC_BotController* AC_Player::GetBotController() {
	return Cast<AC_BotController>(GetController());
}

void AC_Player::OnBotSawPawn(APawn* p_Pawn) {
	if (!IsBot()) return;

	/*AC_Player* l_Player = Cast<AC_Player>(p_Pawn);
	if (l_Player != nullptr) {
		if (l_Player->TeamIndex == TeamIndex) return;

		KnownEnnemiesPositions.Add(GetCloserMapPoint(l_Player->GetActorLocation())->GetActorLocation());

		int l_Life = HealthComponent->GetLife();
		int l_AlliesCount = GetTeammatesAlive();
		if (l_Life < 80) {
			if (l_AlliesCount == 1) {
				if (Money < 1500) {
					MoveToSafePlace();
				}
			}
			else {
				AskForHelpToMates();
			}
		}
		else {
			AttackPawn(l_Player);
			PostKillAction = &AC_Player::JoinMates;
		}
	}*/
}

void AC_Player::AskForHelpToMates() {
	if (!IsBot()) return;

	TArray<AC_Player*> l_Mates = GetMates();
	for (int l_i = 0; l_i < l_Mates.Num(); l_i++) {
		l_Mates[l_i]->AskForHelp(this);
	}
}

void AC_Player::AskForHelp(AC_Player* p_Player) {
	if (!IsBot()) return;

	OnMateAskedForHelp(p_Player);
}

void AC_Player::AttackPawn(APawn* p_Pawn) {
	if (!IsBot()) return;

	AC_Player* l_PawnAsPlayer = Cast<AC_Player>(p_Pawn);

	OnPreAttackPawn(p_Pawn, l_PawnAsPlayer != nullptr);

	if (l_PawnAsPlayer != nullptr) {
		if (l_PawnAsPlayer->TeamIndex == TeamIndex) return;

		AC_BotController* l_Controller = GetBotController();
		l_Controller->MoveToActor(p_Pawn, 500, false, true, true);

		l_Controller->OnBotFinishedMovementEvent.AddDynamic(this, &AC_Player::OnBotMovementEnd);
	}
}

void AC_Player::OnBotMovementEnd() {
	if (!IsBot()) return;

	GetBotController()->OnBotFinishedMovementEvent.RemoveAll(this);
	OnPostAttackPawn(AttackingPawn, Cast<AC_Player>(AttackingPawn) != nullptr);
}

int AC_Player::GetTeammatesAlive() {
	TArray<AActor*, FDefaultAllocator> l_Players;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), l_Players);
	int l_Count = 0;
	for (int l_i = 0; l_i < l_Players.Num(); l_i++) {
		if (Cast<AC_Player>(l_Players[l_i])->TeamIndex == TeamIndex)
			l_Count += 1;
	}
	return l_Count;
}

FEnnemiesNumberData AC_Player::GetEnnemiesAlive() {
	TArray<AActor*, FDefaultAllocator> l_Players;

	FEnnemiesNumberData l_Result;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), l_Players);
	
	for (int l_i = 0; l_i < l_Players.Num(); l_i++) {
		bool l_TeamAlreadyFound = false;
		int l_TeamIndex = 0;
		for (int l_i1 = 0; l_i1 < l_Result.Ennemies.Num(); l_i1++) {
			l_TeamAlreadyFound = l_Result.Ennemies[l_i1]->TeamIndex == Cast<AC_Player>(l_Players[l_i])->TeamIndex;
			if (l_TeamAlreadyFound) {
				l_Result.Ennemies[l_i1]->TeamMembersCount += 1;
				break;
			}
		}

		if (l_TeamAlreadyFound) 
			continue;
		
		UEnnemyTeam* l_Team = CreateDefaultSubobject<UEnnemyTeam>(TEXT("EnnemyTeam"));
		l_Team->TeamIndex = Cast<AC_Player>(l_Players[l_i])->TeamIndex;
		l_Team->TeamMembersCount = 1;	
		l_Result.Ennemies.Add(l_Team);
	}

	return l_Result;
}

void AC_Player::MoveToSafePlace() {
	if (!IsBot()) return;

	
}

TArray<AC_Player*> AC_Player::GetMates() {
	TArray<AActor*, FDefaultAllocator> l_PlayersBase;
	TArray<AC_Player*> l_Results;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), StaticClass(), l_PlayersBase);

	for (int l_i = 0; l_i < l_PlayersBase.Num(); l_i++) {
		if (Cast<AC_Player>(l_PlayersBase[l_i])->TeamIndex != TeamIndex)
			continue;

		l_Results.Add(Cast<AC_Player>(l_PlayersBase[l_i]));
	}

	return l_Results;
}

AMapPoint* AC_Player::GetCloserMapPoint(FVector p_Loc) {
	TArray<AActor*, FDefaultAllocator> l_Points;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AMapPoint::StaticClass(), l_Points);

	FVector l_Closer = FVector();
	AActor* l_CloserPoint = nullptr;

	bool l_IsFirst = false;
	for (int l_i = 0; l_i < l_Points.Num(); l_i++) {
		if (l_IsFirst) {
			l_Closer = l_Points[l_i]->GetActorLocation();
			l_CloserPoint = l_Points[l_i];

			continue;
		}

		if (FVector::Distance(p_Loc, l_Points[l_i]->GetActorLocation()) < FVector::Distance(p_Loc, l_Closer)) {
			l_Closer = l_Points[l_i]->GetActorLocation();
			l_CloserPoint = l_Points[l_i];
		}
	
	}

	return Cast<AMapPoint>(l_CloserPoint);
}

void AC_Player::JoinMates() {
	if (!IsBot()) return;

	TArray<AC_Player*> l_Mates = GetMates();
	int l_Mate = FMath::RandRange(0, l_Mates.Num());
	GetBotController()->MoveToLocation(l_Mates[l_Mate]->GetActorLocation());
}