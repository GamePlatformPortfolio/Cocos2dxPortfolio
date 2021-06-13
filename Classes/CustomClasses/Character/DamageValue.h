#pragma once
class DamageValue {
private:
	int hpDamage;
	int npDamage;

public:
<<<<<<< Updated upstream
	int HpDamage(float num = 0);
	int NpDamage(float num = 0);
	DamageValue() {
		hpDamage = npDamage = 0;
	}
=======
	float HpDamage(float num = 0);
	float NpDamage(float num = 0);
	DamageValue() noexcept { hpDamage = npDamage = 0; }
private:
	float hpDamage;
	float npDamage;
>>>>>>> Stashed changes
};