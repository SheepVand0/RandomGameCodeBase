// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthComponent.h"

// Sets default values for this component's properties
UHealthComponent::UHealthComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}

bool UHealthComponent::ApplyDamages(int p_Amout) {
	if (m_Shield > 0) {
		m_Shield -= p_Amout;

		if (m_Shield < 0) {
			m_Life += m_Shield;
			m_Shield = 0;
		}
	}
	else {
		m_Life -= p_Amout;
		if (m_Life < 0) {
			m_Life = 0;
		}
	}

	OnDamages.Broadcast(m_Life, m_Shield);
	OnLifeChange.Broadcast(m_Life, m_Shield);

	return VerifyLife();
}

void UHealthComponent::Heal(int p_Amout) {
	int l_ShieldAmout = 0;

	if (m_Life < m_BaseLife) {
		m_Life += p_Amout;
		if (m_Life > m_BaseLife) {
			l_ShieldAmout = m_Life + p_Amout - m_BaseLife;
			m_Shield += l_ShieldAmout;
			if (m_Shield > m_BaseShield)
				m_Shield = m_BaseShield;
		}
	}

	OnLifeChange.Broadcast(m_Life, m_Shield);
	OnHeal.Broadcast(m_Life, m_Shield);
}

void UHealthComponent::SetLife(int p_Life, int p_Shield) {

	m_Life = p_Life;
	if (m_Life > m_BaseLife)
		m_Life = m_BaseLife;

	m_Shield = p_Shield;
	if (m_Shield > m_BaseShield)
		m_Shield = m_BaseShield;

	OnLifeChange.Broadcast(m_Life, m_Shield);

	VerifyLife();
}

bool UHealthComponent::VerifyLife() {
	if (m_Life <= 0) {
		OnDeath.Broadcast();
		return false;
	}

	return true;
}

void UHealthComponent::SetBaseLife(int p_Life) {
	m_BaseLife = p_Life;
}

void UHealthComponent::SetBaseShield(int p_Shield) {
	m_BaseShield = p_Shield;
}

int UHealthComponent::GetLife() {
	return m_Life;
}

int UHealthComponent::GetShield() {
	return m_Shield;
}

int UHealthComponent::GetBaseLife() {
	return m_BaseLife;
}

int UHealthComponent::GetBaseShield() {
	return m_BaseShield;
}

// Called every frame
//void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//}

