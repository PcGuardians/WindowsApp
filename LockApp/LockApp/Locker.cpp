#include "Locker.h"
#include <windows.h>

#pragma comment( lib, "user32.lib" )

Locker::Locker() {
}

Locker::~Locker() {
}

void Locker::lockPC() {
	LockWorkStation();
}