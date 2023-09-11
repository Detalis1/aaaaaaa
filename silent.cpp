void __fastcall HOOK_CreateMove(DWORD* ecx, DWORD* edx, int sequence_number, float input_sample_frametime, bool active)
{
  bool ye = false;
	DWORD yaw = 0, pitch = 0;
	yaw = GetMem<DWORD>(client_dll + offsets::my_angle_x);
	pitch = GetMem<DWORD>(client_dll + offsets::my_angle_y);

	if (functions.silent_aimbot && (GetAsyncKeyState(VK_LBUTTON) & 1))
	{
		ye = true;
		RageAim();
	}

	oCreateMove(ecx, edx, sequence_number, input_sample_frametime, active);

	if(ye == true)
	{
		ye = false;
		WriteMem<DWORD>(client_dll + offsets::my_angle_x, &yaw);
		WriteMem<DWORD>(client_dll + offsets::my_angle_y, &pitch);
	}
	

	return;
}
