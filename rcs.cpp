VOID RCS()
{
	static float oX = 0.0f, oY = 0.0f;

	if(GetNumberOfShoots() > 0)
	{

		DWORD dw_my_angle_x = GetMem<DWORD>(client_dll + offsets::my_angle_x) ^ XOR_KEY_ANGLE;
		DWORD dw_my_angle_y = GetMem<DWORD>(client_dll + offsets::my_angle_y) ^ XOR_KEY_ANGLE;

		float my_angle_x = *reinterpret_cast<float*>(&dw_my_angle_x);
		float my_angle_y = *reinterpret_cast<float*>(&dw_my_angle_y);

		
		float punch_x = GetMem<float>(local_player_addr + offsets::punch_x) * 2.0f;
		float punch_y = GetMem<float>(local_player_addr + offsets::punch_y) * 2.0f;

		float new_x = my_angle_x + oX - punch_x;
		float new_y = my_angle_y + oY - punch_y;

		oX = punch_x;
		oY = punch_y;

		DWORD xor_yaw = xorfloat(&new_x, XOR_KEY_ANGLE);
		DWORD xor_pitch = xorfloat(&new_y, XOR_KEY_ANGLE);
		WriteMem<DWORD>(client_dll + offsets::my_angle_x, &xor_yaw);
		WriteMem<DWORD>(client_dll + offsets::my_angle_y, &xor_pitch);
	}
	else
	{
		oX = oY = 0.0f;
	}

	return;
}
