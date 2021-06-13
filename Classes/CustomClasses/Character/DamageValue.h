#pragma once

class DamageValue 
{
public:
	int HpDamage(float num = 0);
	int NpDamage(float num = 0);
	DamageValue() noexcept { hpDamage = npDamage = 0; }
private:
	int hpDamage;
	int npDamage;
};