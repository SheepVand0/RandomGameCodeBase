#include "C_Item.h"

// Sets default values
AC_Item::AC_Item()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	m_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(m_RootComponent);

	m_ItemCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	m_ItemCollision->SetupAttachment(m_RootComponent);
	m_ItemCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	m_ItemCollision->SetCollisionResponseToChannel(ECC_Camera, ECR_Block);
	m_ItemCollision->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	m_ItemProperties = FItemProperties::GetNullItem();

	m_RarityColor.Add(FItemRarity::Normal, FColor(2,155,7));
	m_RarityColor.Add(FItemRarity::Rare, FColor(0,205,215));
	m_RarityColor.Add(FItemRarity::UltraRare, FColor(95, 0, 215));
	m_RarityColor.Add(FItemRarity::Legendary, FColor(255, 209, 0));
}

// Called when the game starts or when spawned
void AC_Item::BeginPlay()
{
	Super::BeginPlay();
}


FItemProperties::FItemProperties() {

	Stackable = false;
	ItemIcon = nullptr;
}

FItemProperties FItemProperties::CreateItemProperties(TSubclassOf<AC_Item> p_ItemClass, FString p_ItemName, FString p_Category, FTransform p_HandedTransform, bool p_Stackable, UTexture2D* p_ItemIcon) {

	FItemProperties l_NewItem;
	l_NewItem.ItemClass = p_ItemClass;
	l_NewItem.ItemName = p_ItemName;
	l_NewItem.Category = p_Category;
	l_NewItem.HandedTransform = p_HandedTransform;
	l_NewItem.Stackable = p_Stackable;
	l_NewItem.ItemIcon = p_ItemIcon;

	return l_NewItem;
}

FItemProperties FItemProperties::GetNullItem() {
	return CreateItemProperties(nullptr, FString("UNDEFINED"), FString(""), * new FTransform(), false, nullptr);
}

bool FItemProperties::IsNullItem(FItemProperties p_Item) {
	return FItemProperties::Equal(p_Item, GetNullItem());
}

bool FItemProperties::Equal(FItemProperties p_Properties1, FItemProperties p_Properties2) {

	return (
		p_Properties1.ItemClass == p_Properties2.ItemClass
		&& p_Properties1.ItemName == p_Properties2.ItemName
		&& p_Properties1.Category == p_Properties2.Category
		&& p_Properties1.Stackable == p_Properties2.Stackable
		&& p_Properties1.ItemIcon == p_Properties2.ItemIcon);
}

// Called every frame
void AC_Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AC_Item::SetItemProperties(FItemProperties p_Properties) {
	m_ItemProperties = p_Properties;
}

void AC_Item::Construct(FItemProperties p_Properties, FTransform p_CollisionTransform, UStaticMeshComponent* p_RootStaticMesh, USkeletalMeshComponent* p_RootSkeletalMesh, bool p_SimulatePhysicsOnDrop, FItemRarity p_Rarity) {
	SetItemProperties(p_Properties);
	m_ItemCollision->SetRelativeTransform(p_CollisionTransform);
	m_RootStaticMesh = p_RootStaticMesh;
	m_RootSkeletalMesh = p_RootSkeletalMesh;
	m_SimulatePhysicsOnDrop = p_SimulatePhysicsOnDrop;
	m_ItemRarity = p_Rarity;

	if (m_ParentPlayer == nullptr)
		SetItemSimulatePhysics(m_SimulatePhysicsOnDrop);
}

void AC_Item::Use() {
	m_IsUsing = true;
	OnUse();
}

void AC_Item::StopUse() {
	m_IsUsing = false;
	OnStopUse();
}

void AC_Item::OnUse() {}

void AC_Item::OnStopUse() {}

bool AC_Item::IsUsing() {
	return m_IsUsing;
}

void AC_Item::Select(ACharacter* p_Parent, USceneComponent* p_AttachObject) {
	m_ParentPlayer = p_Parent;
	SetItemSimulatePhysics(false);
	SetActorLocation(p_AttachObject->GetComponentLocation(), false);
	AttachToComponent(p_AttachObject, FAttachmentTransformRules::FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, EAttachmentRule::SnapToTarget, false));
	OnItemSelected();
	SetChildsHasCollision(ECollisionEnabled::NoCollision);
}

void AC_Item::UnSelect() {
	DetachFromActor(FDetachmentTransformRules::FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));
	SetItemSimulatePhysics(m_SimulatePhysicsOnDrop);
	m_ParentPlayer = nullptr;
	OnItemUnselected();
	SetChildsHasCollision(ECollisionEnabled::QueryAndPhysics);
}

void AC_Item::SetChildsHasCollision(ECollisionEnabled::Type p_Enabled) {
	TSet<UActorComponent*> l_Components = GetComponents();
	TArray<UActorComponent*> l_ComponentsArray = l_Components.Array();
	for (int l_i = 0; l_i < l_ComponentsArray.Num();l_i++)
	{
		USkeletalMeshComponent* l_SkMesh = Cast<USkeletalMeshComponent>(l_ComponentsArray[l_i]);
		if (l_SkMesh != nullptr) {
			l_SkMesh->SetCollisionEnabled(p_Enabled);
			continue;
		}

		UStaticMeshComponent* l_StaticMesh = Cast<UStaticMeshComponent>(l_ComponentsArray[l_i]);
		if (l_StaticMesh != nullptr) {
			l_StaticMesh->SetCollisionEnabled(p_Enabled);
		}
	}
}

void AC_Item::OnItemSelected() {
}

void AC_Item::OnItemUnselected() {
}

FItemProperties AC_Item::GetPropertiesCopy() {
	FItemProperties l_NewProperties = FItemProperties::CreateItemProperties(m_ItemProperties.ItemClass, m_ItemProperties.ItemName, m_ItemProperties.Category, m_ItemProperties.HandedTransform, m_ItemProperties.Stackable, m_ItemProperties.ItemIcon);
	l_NewProperties.RemovableFromInventory = m_ItemProperties.RemovableFromInventory;
	return l_NewProperties;
}

const FColor AC_Item::GetRarityColor(FItemRarity p_Rarity) {
	if (!m_RarityColor.Contains(p_Rarity))
		return FColor(m_DefaultRarityColor->R, m_DefaultRarityColor->G, m_DefaultRarityColor->B);

	const FColor* l_ReturnedColor = m_RarityColor.Find(p_Rarity);
	return FColor(l_ReturnedColor->R, l_ReturnedColor->G, l_ReturnedColor->B);
}

void AC_Item::SetItemSimulatePhysics(bool p_IsSimulating) {
	if (m_RootStaticMesh != nullptr) {
		m_RootStaticMesh->SetSimulatePhysics(p_IsSimulating);
	}

	if (m_RootSkeletalMesh != nullptr) {
		m_RootSkeletalMesh->SetSimulatePhysics(p_IsSimulating);
	}
}

void AC_Item::Drop() {
	FDetachmentTransformRules* l_Rules = new FDetachmentTransformRules(EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, true);
	DetachFromActor(*l_Rules);
	SetItemSimulatePhysics(m_SimulatePhysicsOnDrop);
}

void AC_Item::NotifyEvent(FString p_EventName) {

}