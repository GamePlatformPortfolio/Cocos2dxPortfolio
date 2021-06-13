#include "CustomClasses/Character/DamageValue.h"

<<<<<<< Updated upstream
int DamageValue::HpDamage(float num) {
	if (num == 0) return hpDamage;
	hpDamage = num;
	return hpDamage;
}
int DamageValue::NpDamage(float num) {
	if (num == 0)return npDamage;
	if (num < 0) num = 1;
=======
float DamageValue::HpDamage(float num) 
{
	if (num == 0) 
		return hpDamage;

	hpDamage = num;
	return hpDamage;
}
float DamageValue::NpDamage(float num) 
{
	if (num == 0)
		return npDamage;

>>>>>>> Stashed changes
	npDamage = num;
	return npDamage;
}