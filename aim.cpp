VOID RageAim()
{
	entity_item target_entity;
	ZeroMemory(&target_entity, sizeof(target_entity));

	float min_dist = 10000;

	DWORD my_team = GetMem<DWORD>(local_player_addr + offsets::team);
	Vector3 my_pos = GetMem<Vector3>(local_player_addr + offsets::read_position);
	if (my_pos.IsZero())
		return;
	int index = 0;

	for (int i = 0; i < entities.size(); ++i)
	{
	get:

		entity_item ent_item;
		ent_item.address = entities[i];

		if (!EntityValid(ent_item.address))
			continue;

		DWORD ent_team = GetMem<DWORD>(ent_item.address + offsets::team);
		if (my_team == ent_team)
			continue;

		Vector3 ent_pos = GetBonePos(ent_item.address, 14);
		Vector2 screen_ent_pos;
		if (ent_pos.IsZero() || !Screen(&ent_pos, &screen_ent_pos))
		{
			continue;
		}

		Vector2 p = GetCrosshairPos();
		float screen_dist = sqrtf(powf(p.x - screen_ent_pos.x, 2) + powf(p.y - screen_ent_pos.y, 2));
		if (screen_dist > settings.aimbot_radius)
			continue;

	//	float dist = GetDistance(my_pos, ent_pos);
		float dist = screen_dist;
		if (dist < min_dist)
		{
			min_dist = dist;
			index = i;
			target_entity.address = ent_item.address;
		}
	}

	if (target_entity.address == NULL)
	{
		return;
	}

	DWORD dw_my_angle_x = GetMem<DWORD>(client_dll + offsets::my_angle_x) ^ XOR_KEY_ANGLE;
	DWORD dw_my_angle_y = GetMem<DWORD>(client_dll + offsets::my_angle_y) ^ XOR_KEY_ANGLE;

	float my_angle_x = *reinterpret_cast<float*>(&dw_my_angle_x);
	float my_angle_y = *reinterpret_cast<float*>(&dw_my_angle_y);


	float angle_x_with_punch = GetMem<float>(client_dll + 0x13E3D60);
	float angle_y_with_punch = GetMem<float>(client_dll + 0x13E3D5C);

	float delta_x = angle_x_with_punch - my_angle_x;
	float delta_y = angle_y_with_punch - my_angle_y;

	Vector3 ent_pos = GetBonePos(target_entity.address, 14);

	if (ent_pos.IsZero())
	{
		return;
	}

	ent_pos.z += 1.0f;
	//ent_pos = GetMem<Vector3>(target_entity.address + offsets::read_position);
	//ent_pos.z += 10.0f;

	Vector2 s_pos;
	Screen(&ent_pos, &s_pos);
	if (s_pos.IsZero())
	{
		return;
	}
	if (GetAsyncKeyState(VK_CONTROL)) // костыль на время
		my_pos.z += 47.0f;
	else 
		my_pos.z += 70;

	float x = ent_pos.x - my_pos.x;
	float y = ent_pos.y - my_pos.y;
	float z = ent_pos.z - my_pos.z;
	float dist = GetDistance(my_pos, ent_pos);

	static float oX = 0, oY = 0;

	float new_yaw = atan2f(y, x) * (180.0f / 3.1415926f);
	float new_pitch = -asinf(z / dist) * (180.0f / 3.1415926f);

	new_yaw = new_yaw + oX - (delta_x * 2);
	new_pitch = new_pitch + oY - (delta_y * 2);
	oX = delta_x;
	oY = delta_y;

	float d_x = (new_yaw - my_angle_x) / settings.aim_smooth;
	float d_y = (new_pitch - my_angle_y) / settings.aim_smooth;

	my_angle_x += d_x;
	my_angle_y += d_y;

	DWORD xor_yaw = xorfloat(&my_angle_x, XOR_KEY_ANGLE);
	DWORD xor_pitch = xorfloat(&my_angle_y, XOR_KEY_ANGLE);
	WriteMem<DWORD>(client_dll + offsets::my_angle_x, &xor_yaw);
	WriteMem<DWORD>(client_dll + offsets::my_angle_y, &xor_pitch);

	return;
}
