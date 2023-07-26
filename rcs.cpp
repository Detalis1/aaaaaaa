VOID RCS()
{
	if(GetNumberOfShoots() > 1)
	{
		static float oX = 0, oY = 0;

		DWORD dw_my_angle_x = GetMem<DWORD>(client_dll + offsets::my_angle_x) ^ XOR_KEY_ANGLE;
		DWORD dw_my_angle_y = GetMem<DWORD>(client_dll + offsets::my_angle_y) ^ XOR_KEY_ANGLE;

		float my_angle_x = *reinterpret_cast<float*>(&dw_my_angle_x);
		float my_angle_y = *reinterpret_cast<float*>(&dw_my_angle_y);

		float angle_x_with_punch = GetMem<float>(client_dll + 0x13E3D60);
		float angle_y_with_punch = GetMem<float>(client_dll + 0x13E3D5C);

		float punch_x = (angle_x_with_punch - my_angle_x) * 2;
		float punch_y = (angle_y_with_punch - my_angle_y) * 2;

		float new_x = my_angle_x + oX - punch_x;
		float new_y = my_angle_y + oY - punch_y;

		oX = punch_x;
		oY = punch_y;

		DWORD xor_yaw = xorfloat(&new_x, XOR_KEY_ANGLE);
		DWORD xor_pitch = xorfloat(&new_y, XOR_KEY_ANGLE);
		WriteMem<DWORD>(client_dll + offsets::my_angle_x, &xor_yaw);
		WriteMem<DWORD>(client_dll + offsets::my_angle_y, &xor_pitch);
	}

	return;
}
