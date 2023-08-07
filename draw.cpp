void DrawLine(LPDIRECT3DDEVICE9 pDevice, int x1, int y1, int x2, int y2, int thickess, D3DCOLOR color, bool obvodka = false)
{
	
	if (LineL == nullptr)
	{
		D3DXCreateLine(pDevice, &LineL);
	}

	if (LineL != nullptr)
	{

		D3DXVECTOR2 Line[2];
		Line[0] = D3DXVECTOR2(x1, y1);
		Line[1] = D3DXVECTOR2(x2, y2);

		LineL->SetWidth(thickess);
		LineL->Draw(Line, 2, color);

		if (obvodka)
		{
			LineL->SetWidth(1);
			int x = x2 - x1;
			int y = y2 - y1;

			if (abc(y) > abc(x))
			{
				Line[0] = D3DXVECTOR2(x1 + 1, y1);
				Line[1] = D3DXVECTOR2(x2 + 1, y2);
				LineL->Draw(Line, 2, d3dcolor::Black);
				Line[0] = D3DXVECTOR2(x1 - 1, y1);
				Line[1] = D3DXVECTOR2(x2 - 1, y2);
				LineL->Draw(Line, 2, d3dcolor::Black);
			}
			else
			{
				Line[0] = D3DXVECTOR2(x1, y1 + 1);
				Line[1] = D3DXVECTOR2(x2, y2 + 1);
				LineL->Draw(Line, 2, d3dcolor::Black);
				Line[0] = D3DXVECTOR2(x1, y1 - 1);
				Line[1] = D3DXVECTOR2(x2, y2 - 1);
				LineL->Draw(Line, 2, d3dcolor::Black);
			}

			Line[0] = D3DXVECTOR2(x1, y1);
			Line[1] = D3DXVECTOR2(x2, y2);

		}
	}
	//LineL->Release();
	return;
}
