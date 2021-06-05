#pragma once
class DamageValue {
private:
	int hpDamage;
	int npDamage;

public:
	int HpDamage(float num = 0);
	int NpDamage(float num = 0);
	DamageValue() {
		hpDamage = npDamage = 0;
	}
};