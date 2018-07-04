
double Rms(short int *wave, int length)
{
	double sumSquared = 0;
	double scaleShortToDouble = 1.0 / 0x8000;

	for (int i = 0; i < length; i++)
	{
		double s = wave[i] * scaleShortToDouble;
		sumSquared += s * s;
	}
	return sqrtl(2) * sqrt(sumSquared / length);
}

int isAvg(short int *wave, int NUMPTS)
{
	int avg = 0;
	for (int i = 0; i < NUMPTS; i++)
	{
		if (wave[i] < 0)
			avg = avg + (wave[i]) * -1;

		else
			avg = avg + (wave[i]);
	}

	avg = avg / NUMPTS;
	return avg;
}

bool MIC_ALIVE()
{
	int x = 12;
	int seconds = 1;
	int sampleRate = 44100;

	int NUMPTS = sampleRate * seconds;
	short int *waveIn = new short int[NUMPTS];

	HWAVEIN hWaveIn;
	WAVEHDR WaveInHdr;
	MMRESULT result;
	HWAVEOUT hWaveOut;

	WAVEFORMATEX pFormat;
	pFormat.wFormatTag = WAVE_FORMAT_PCM;
	pFormat.nChannels = 1;
	pFormat.nSamplesPerSec = sampleRate;
	pFormat.nAvgBytesPerSec = 2 * sampleRate;
	pFormat.nBlockAlign = 2;
	pFormat.wBitsPerSample = 16;
	pFormat.cbSize = 0;

	result = waveInOpen(&hWaveIn, WAVE_MAPPER, &pFormat, 0, 0, WAVE_FORMAT_DIRECT);

	if (result) // Error "device ID has been used that is out of range for your system"
		return FALSE;

	WaveInHdr.lpData = (LPSTR)waveIn;
	WaveInHdr.dwBufferLength = 2 * NUMPTS;
	WaveInHdr.dwBytesRecorded = 0;
	WaveInHdr.dwUser = 0;
	WaveInHdr.dwFlags = 0;
	WaveInHdr.dwLoops = 0;

	for (int c = 0; c <= x; c++)
	{
		waveInPrepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));
		result = waveInAddBuffer(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

		result = waveInStart(hWaveIn);
		if (result)
		{
			return FALSE;
		}

		// Wait until finished recording 
		while ((WaveInHdr.dwFlags & WHDR_DONE) == 0)
			Sleep(100);

		waveInUnprepareHeader(hWaveIn, &WaveInHdr, sizeof(WAVEHDR));

		double rms_sound = Rms(waveIn, NUMPTS); // convert raw to magnitude
		int dbs = 20 * log10(rms_sound); // convert magnitude to dB

		static int old_value = NULL;

		if (dbs != old_value && c > 2)
			return TRUE; // OK is ALIVE

		old_value = dbs;

		printf("dBs = %i | RMS (Root mean square) = %d | old_value = %i\n", dbs, rms_sound, old_value); // For DBG
	}

	return FALSE;
}
