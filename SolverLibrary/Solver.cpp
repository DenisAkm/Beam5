
#include "stdafx.h"
#include "omp.h"
#include <iostream>
#include <string>
#include <fstream>
#include "cstdlib"
#include "iomanip"
#include <complex>


using namespace std;

string version = "v1";
//Определение физических констант
const double pi = 3.14159265358979323846;
const double Mu_0 = 4.0f * pi * 1.0e-7f;                                    // 1.0 / (c * c * E_0) Гн/м     magnetic constant    магнитная постоянная
const double E_0 = 8.85418781761e-12f;                                      // 8.85e-12 Ф/м        electric constant     электрическая постоянная
static double с0 = 1 / (float)sqrt(E_0 * Mu_0);       // м/с скорость света       
///Костины значения
//static double pi = 3.1415926535897932384626433832795;
//static double с0 = 3.0e8;
//static double E_0 = 1.0 / с0 / 60.0 / 2.0 / pi;
//static double Mu_0 = 1.0 / (с0 * с0 * E_0);


struct CComplex
{
	double re;
	double im;
};
double AbsComplex(CComplex a)
{
	return sqrt(a.re*a.re + a.im*a.im);
}
static int GetStartIndex(int cur_proc, int proc, int size)
{
	int eq_number = size / proc;
	int overflow_eq = size % proc;
	if (overflow_eq != 0)
	{
		eq_number++;
	}
	int cur_cor_order = 0;
	if (overflow_eq != 0 && cur_proc > overflow_eq)
	{
		cur_cor_order = cur_proc - overflow_eq;
	}
	int start = cur_proc * eq_number - cur_cor_order;
	return start;
}
static int GetEndIndex(int cur_proc, int proc, int size)
{
	cur_proc++;
	int eq_number = size / proc;
	int overflow_eq = size % proc;
	if (overflow_eq != 0)
	{
		eq_number++;
	}
	int cur_cor_order = 0;
	if (overflow_eq != 0 && cur_proc > overflow_eq)
	{
		cur_cor_order = cur_proc - overflow_eq;
	}
	int end = cur_proc * eq_number - 1 - cur_cor_order;
	return end;
}
CComplex operator /(CComplex a, CComplex b)
{
	CComplex res;
	res.re = (a.re*b.re + a.im*b.im) / (b.re*b.re + b.im*b.im);
	res.im = (a.im*b.re - a.re*b.im) / (b.re*b.re + b.im*b.im);
	return res;
}
CComplex operator /(double a, CComplex b)
{
	CComplex res;	
	double m = b.re*b.re + b.im*b.im;
	res.re = a*b.re / m;
	res.im = -a*b.im / m;
	return res;
}
CComplex operator /(CComplex a, double b)
{
	CComplex res;
	res.re = a.re / b;
	res.im = a.im / b;
	return res;
}
CComplex operator -(CComplex a, CComplex b)
{
	CComplex res;
	res.re = a.re - b.re;
	res.im = a.im - b.im;
	return res;

}
CComplex operator -(CComplex a, double b)
{
	CComplex res;
	res.re = a.re - b;
	res.im = a.im;
	return res;
}
CComplex operator -(double a, CComplex b)
{
	CComplex res;
	res.re = a - b.re;
	res.im = (-1) * b.im;
	return res;
}
CComplex operator +(CComplex a, CComplex b)
{
	CComplex res;
	res.re = a.re + b.re;
	res.im = a.im + b.im;
	return res;
}
CComplex operator +(double a, CComplex b)
{
	CComplex res;
	res.re = a + b.re;
	res.im = b.im;
	return res;
}
CComplex operator * (CComplex a, CComplex b)
{
	CComplex res;
	res.re = a.re*b.re - a.im*b.im;
	res.im = a.im*b.re + a.re*b.im;
	return res;
}
CComplex operator *(CComplex a, double b)
{
	CComplex res;
	res.re = a.re*b;
	res.im = a.im*b;
	return res;
}
CComplex operator *(double a, CComplex b)
{
	CComplex res;
	res.re = a*b.re;
	res.im = a*b.im;
	return res;
}
CComplex Conjugate(CComplex c)
{
	CComplex res;
	res.re = c.re;
	res.im = (-1) * c.im;
	return res;
}
//Модуль комплексного числа

CComplex Csqrt(CComplex v)
{
	CComplex res;
	std::complex< double > z(v.re, v.im);
	std::complex< double > r = sqrt(z);
	res.re = r.real();
	res.im = r.imag();
	/*double r, phi, R, Phi;
	phi = atan2(v.im, v.re);
	r = sqrt(v.re * v.re + v.im * v.im);
	Phi = phi / 2;
	R = sqrt(r);
	res.re = R * cos(Phi);
	res.im = R * sin(Phi);*/
	return res;
}
CComplex Csqrt(double v)
{
	CComplex val;
	val.re = v;
	val.im = 0;
	return Csqrt(val);
}
CComplex Sinh(CComplex v)
{
	CComplex res;
	std::complex< double > z(v.re, v.im);
	std::complex< double > r = sinh(z);
	res.re = r.real();
	res.im = r.imag();
	//double expx = exp(v.re);
	//double expxM = exp((-1) * v.re);
	//res.re = 1 / 2 * (expx - expxM) * cos(v.im);
	//res.im = 1 / 2 * (expx + expxM) * sin(v.im);

	return res;
}
CComplex Cosh(CComplex v)
{
	CComplex res;

	std::complex< double > z(v.re, v.im);
	std::complex< double > r = cosh(z);
	/*double expx = exp(v.re);
	double expxM = exp((-1) * v.re);
	res.re = 1 / 2 * (expx + expxM) * cos(v.im);
	res.im = 1 / 2 * (expx - expxM) * sin(v.im);*/
	res.re = r.real();
	res.im = r.imag();
	return res;
	return res;
}

static int roundInt(double x)
{
	if (x < 0) x -= 0.5;
	else x += 0.5;
	return (int)x;
}

struct myVectorC
{
	CComplex x;
	CComplex y;
	CComplex z;
};
struct myVector
{
	double x;
	double y;
	double z;

	double Modulus()
	{
		return sqrt(x * x + y * y + z * z);
	}
};
void LogInfo(double mes1, double mes2, double mes3, double mes4)
{
	std::ofstream out("C:\\Users\\User\\source\\repos\\RadomeRadar_Beta_2019\\Samples\\Alex\\log.txt", std::ios::app);

	if (out.is_open())
	{		
		out << to_string(mes1) << "\t" << to_string(mes2) << "\t" << to_string(mes3) << "\t" << to_string(mes4) << "\n" << std::endl;
	}
	out.close();
}
bool isColinear(myVector a, myVector b, int precision)
{
	bool ans = false;
	double param = pow(10, (-1) * precision);
	if (abs(b.x - a.x) < param && abs(b.y - a.y) < param && abs(b.z - a.z) < param)
	{
		ans = true;
	}
	return ans;
}
CComplex ScalC(myVectorC v1, myVectorC v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
CComplex ScalC(myVector v1, myVectorC v2)
{
	return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}
myVector operator -(myVector v1, myVector v2)
{
	myVector res;
	res.x = v1.x - v2.x;
	res.y = v1.y - v2.y;
	res.z = v1.z - v2.z;
	return res;
}
myVector Normalize(myVector a)
{
	myVector ans;
	double length = sqrt(a.x * a.x + a.y * a.y + a.z * a.z);
	ans.x = a.x / length;
	ans.y = a.y / length;
	ans.z = a.z / length;
	return ans;
}
myVector Cross(myVector v1, myVector v2)
{
	myVector ans;
	ans.x = v1.y * v2.z - v1.z * v2.y;
	ans.y = v1.z * v2.x - v1.x * v2.z;
	ans.z = v1.x * v2.y - v1.y * v2.x;
	return ans;
}
double Scal(myVector v1, myVector v2)
{
	double ans = 0;
	if (isColinear(v1, v2, 9))
	{
		ans = 1;
	}
	else
	{
		ans = v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
	}
	return ans;
}
myVector operator *(double a, myVector b)
{
	myVector ans;
	ans.x = a * b.x;
	ans.y = a * b.y;
	ans.z = a * b.z;
	return ans;
}


static double GetThetaGlobal(double phi, double theta, int system)
{
	double a = 0; double b = 0; double c = 0;
	const double pi = 3.14159265358979323846;
	if (system == 0)
	{
		a = sin(theta * pi / 180) * cos(phi * pi / 180);
		b = sin(theta * pi / 180) * sin(phi * pi / 180);
		c = cos(theta * pi / 180);
	}
	else if (system == 1)
	{
		c = sin(theta * pi / 180) * cos(phi * pi / 180);
		a = sin(theta * pi / 180) * sin(phi * pi / 180);
		b = cos(theta * pi / 180);
	}
	else if (system == 2)
	{
		b = sin(theta * pi / 180) * cos(phi * pi / 180);
		c = sin(theta * pi / 180) * sin(phi * pi / 180);
		a = cos(theta * pi / 180);
	}

	double m = c / sqrt(a * a + b * b + c * c);
	if (m < -1)
	{
		m = -1;
	}
	else if (m > 1)
	{
		m = 1;
	}

	return acos(m) / pi * 180;
}
static double GetPhiGlobal(double phi, double theta, int system)
{
	double a = 0; double b = 0; double c = 0;
	const double pi = 3.14159265358979323846;
	if (system == 0)
	{
		a = sin(theta * pi / 180) * cos(phi * pi / 180);
		b = sin(theta * pi / 180) * sin(phi * pi / 180);
		c = cos(theta * pi / 180);
	}
	else if (system == 1)
	{
		c = sin(theta * pi / 180) * cos(phi * pi / 180);
		a = sin(theta * pi / 180) * sin(phi * pi / 180);
		b = cos(theta * pi / 180);
	}
	else if (system == 2)
	{
		b = sin(theta * pi / 180) * cos(phi * pi / 180);
		c = sin(theta * pi / 180) * sin(phi * pi / 180);
		a = cos(theta * pi / 180);
	}
	double m = c / sqrt(a * a + b * b + c * c);
	if (m < -1)
	{
		m = -1;
	}
	else if (m > 1)
	{
		m = 1;
	}

	double thetaGlobal = acos(m) / pi * 180;
	if (thetaGlobal == 0)
	{
		return 0;
	}
	else
	{
		double v = a / sin(thetaGlobal * pi / 180);
		if (v > 1)
		{
			v = 1;
		}
		else if (v < -1)
		{
			v = -1;
		}

		double phiGlobal = acos(v) / pi * 180;
		//if (theta > 90 && theta < 270)
		//{
		//    return 360 - phiGlobal;
		//}
		//else
		//{
		//    return phiGlobal;
		//}

		if (b < 0)
		{
			return 360 - phiGlobal;
		}
		else
		{
			return phiGlobal;
		}
	}
}

extern "C" __declspec(dllexport)
string GetSolverVersion() 
{
	return version;
}
//C# передаёт двумерный массив в одномерном виде. Преобразование индексов [i][j] -> [0][i*np+j]
extern "C" __declspec(dllexport) 
CComplex* SurfaceCurrentToGeometry(	double* ApX, double* ApY, double* ApZ, double* ApS, 
									CComplex* Ix, CComplex* Iy, CComplex* Iz, 
									CComplex* Mx, CComplex* My, CComplex* Mz,
									double* GeoX, double* GeoY, double* GeoZ,
									double freq, int ApSize, int GeoSize, int &pthread)
{
#ifndef _OPENMP
	std::cout << "OpenMP is not supported!\n";
	return nullptr;
#endif	
		
	//В С# передаётся "спресованный" одномерный массив, где на позициях {0,1,2} - Ex, Ey, Ez; {3,4,5} - Hx, Hy, Hz;
	int totalNumber = GeoSize * 6;
	CComplex* solution = new CComplex[totalNumber];
	
	for (int i = 0; i < totalNumber; i++)
	{
		solution[i].re = 0;
		solution[i].im = 0;
	}
	
	//установка числа потоков
	if (omp_get_max_threads() < pthread)
	{
		pthread = omp_get_max_threads();
	}
	omp_set_num_threads(pthread);
	
	
	
	
	static CComplex imOne;
	imOne.re = 0;
	imOne.im = 1;
	double Lambda = с0 / freq;
	double Omega = 2 * pi * freq;
	double K_0 = 2 * pi / Lambda;      // волновое число 2pi/lambda
	double K2 = K_0 * K_0;
	CComplex iomegaE = imOne * Omega * E_0;
	CComplex iomegaMu = imOne * Omega * Mu_0;

	CComplex Ekoeff = (-1.0) / iomegaE;          // 1/i*omega*E_0
	CComplex Mukoeff = (-1.0) / iomegaMu;         // -1/i*omega*Mu_0

	///////////////	
#pragma omp parallel shared (K_0, K2, imOne, Ekoeff, Mukoeff, ApX, ApY, ApZ, ApS, Ix, Iy, Iz, Mx, My, Mz, GeoX, GeoY, GeoZ, freq, ApSize, GeoSize)
	{
		int proc = omp_get_num_threads();		//количество потоков
		int cur_proc = omp_get_thread_num();	//идентификация текущего потока

		int start = GetStartIndex(cur_proc, proc, GeoSize);
		int end = GetEndIndex(cur_proc, proc, GeoSize);

		for (int j = start; j <= end; j++)
		{
			//
			//  Переменные
			//                        
			CComplex ex, ey, ez, hx, hy, hz;
			ex.re = 0;
			ey.re = 0;
			ez.re = 0;
			hx.re = 0;
			hy.re = 0;
			hz.re = 0;

			ex.im = 0;
			ey.im = 0;
			ez.im = 0;
			hx.im = 0;
			hy.im = 0;
			hz.im = 0;

			double xsecond = GeoX[j];
			double ysecond = GeoY[j];
			double zsecond = GeoZ[j];


			for (int t = 0; t < ApSize; t++)
			{
				double xprime = ApX[t];
				double yprime = ApY[t];
				double zprime = ApZ[t];

				double square = ApS[t];

				CComplex ix = Ix[t];
				CComplex iy = Iy[t];
				CComplex iz = Iz[t];

				CComplex mx = Mx[t];
				CComplex my = My[t];
				CComplex mz = Mz[t];

				double x_x0 = xprime - xsecond;
				double y_y0 = yprime - ysecond;
				double z_z0 = zprime - zsecond;
				double x2 = x_x0 * x_x0;
				double y2 = y_y0 * y_y0;
				double z2 = z_z0 * z_z0;

				double r = sqrt(x2 + y2 + z2);

				//////////////////////////
				//
				//  Переменные
				//
				CComplex exp_ikr;
				exp_ikr.re = cos(K_0 * r);
				exp_ikr.im = (-1)*sin(K_0 * r);

				CComplex funG = exp_ikr / (4 * pi * r);
				double r2 = r * r;
				double r3 = r * r2;
				double r4 = r2 * r2;

				//
				// НАЧАЛО КОСТИНЫ ФОРМУЛЫ
				//
				CComplex coeffA = (-1) * imOne * K_0 / r - 1.0 / r2;
				CComplex coeffB = (3.0 + 3.0 * imOne * K_0 * r - K2 * r2) / r4;

				CComplex dx = x_x0 * coeffA * funG;
				CComplex dy = y_y0 * coeffA * funG;
				CComplex dz = z_z0 * coeffA * funG;

				CComplex dxx = (x2 * coeffB + coeffA) * funG;
				CComplex dyy = (y2 * coeffB + coeffA) * funG;
				CComplex dzz = (z2 * coeffB + coeffA) * funG;

				CComplex dxy = (x_x0 * y_y0 * coeffB) * funG;
				CComplex dxz = (x_x0 * z_z0 * coeffB) * funG;
				CComplex dyz = (y_y0 * z_z0 * coeffB) * funG;
				//
				// КОНЕЦ
				//

				//ex = ex - Ekoeff * (K2 * ix * funG + ix * dxx + iy * dxy + iz * dxz) * square + (mz * dy - my * dz) * square; //-
				//ey = ey - Ekoeff * (K2 * iy * funG + iy * dyy + iz * dyz + ix * dxy) * square + (mx * dz - mz * dx) * square; //-
				//ez = ez - Ekoeff * (K2 * iz * funG + iz * dzz + ix * dxz + iy * dyz) * square + (my * dx - mx * dy) * square; //-

				//hx = hx - Mukoeff * (K2 * mx * funG + mx * dxx + my * dxy + mz * dxz) * square - (iz * dy - iy * dz) * square; //+
				//hy = hy - Mukoeff * (K2 * my * funG + mx * dxy + my * dyy + mz * dyz) * square - (ix * dz - iz * dx) * square; //+
				//hz = hz - Mukoeff * (K2 * mz * funG + mx * dxz + my * dyz + mz * dzz) * square - (iy * dx - ix * dy) * square; //+

				ex = ex + Ekoeff * (K2 * ix * funG + ix * dxx + iy * dxy + iz * dxz) * square - (mz * dy - my * dz) * square; //-
				ey = ey + Ekoeff * (K2 * iy * funG + iy * dyy + iz * dyz + ix * dxy) * square - (mx * dz - mz * dx) * square; //-
				ez = ez + Ekoeff * (K2 * iz * funG + iz * dzz + ix * dxz + iy * dyz) * square - (my * dx - mx * dy) * square; //-

				hx = hx + Mukoeff * (K2 * mx * funG + mx * dxx + my * dxy + mz * dxz) * square + (iz * dy - iy * dz) * square; //+
				hy = hy + Mukoeff * (K2 * my * funG + mx * dxy + my * dyy + mz * dyz) * square + (ix * dz - iz * dx) * square; //+
				hz = hz + Mukoeff * (K2 * mz * funG + mx * dxz + my * dyz + mz * dzz) * square + (iy * dx - ix * dy) * square; //+				
				//////////////////////////
			}
			solution[6 * j + 0] = ex;
			solution[6 * j + 1] = ey;
			solution[6 * j + 2] = ez;
			solution[6 * j + 3] = hx;
			solution[6 * j + 4] = hy;
			solution[6 * j + 5] = hz;
		}
	}
		
		return solution;	
}


extern "C" __declspec(dllexport) 
CComplex* ApertureFF(CComplex* Ix, CComplex* Iy, CComplex* Iz, CComplex* Mx, CComplex* My, CComplex* Mz,
	int appSize, double* trX, double* trY, double* trZ, double* trS,
	double freq, double thetaStart, double thetaFinish, double thetaStep, double phiStart, double phiFinish, double phiStep, int systemOfCoord, int proc)
{
#ifndef _OPENMP
	std::cout << "OpenMP is not supported!\n";
	return nullptr;
#endif	

	//установка числа потоков
	if (omp_get_max_threads() < proc)
	{
		proc = omp_get_max_threads();
	}
	omp_set_num_threads(proc);

	//Определение физических констант	
	double Omega = 2 * pi * freq;
	double K_0 = Omega / с0;
	double K2 = K_0 * K_0;
	double pi180 = pi / 180;
	static CComplex imOne;
	imOne.re = 0;
	imOne.im = 1;
	CComplex Ekoeff = (1.0) / (4 * pi * imOne * Omega * E_0); //fourComx*piComx*imOne*omegaComx*E_0Comx                            


	//Расчёт
	int numberPhi = roundInt((phiFinish - phiStart) / phiStep) + 1;
	int numberTheta = roundInt((thetaFinish - thetaStart) / thetaStep) + 1;
	int numberTotal = numberPhi * numberTheta;

	int counterTotal = 3 * numberTotal;
	CComplex* sol = new CComplex[counterTotal];

	for (int i = 0; i < counterTotal; i++)
	{
		sol[i].re = 0;
		sol[i].im = 0;
	}

#pragma omp parallel shared (Ix, Iy, Iz, Mx, My, Mz, appSize, trX, trY, trZ, trS, freq, thetaStart, thetaFinish, thetaStep, phiStart, phiFinish, phiStep, systemOfCoord, proc, pi180)
	{
		int cur_proc = omp_get_thread_num();	//идентификация текущего потока
		int start = GetStartIndex(cur_proc, proc, numberTotal);
		int end = GetEndIndex(cur_proc, proc, numberTotal);

		for (int j = start; j <= end; j++)
		{
			double phi = phiStart + (j % numberPhi) * phiStep;
			double theta = thetaStart + j / numberPhi * thetaStep;

			double thetaGlobal = GetThetaGlobal(phi, theta, systemOfCoord);
			double phiGlobal = GetPhiGlobal(phi, theta, systemOfCoord);

			CComplex ex;
			CComplex ey;
			CComplex ez;
			ex.re = 0; ey.re = 0; ez.re = 0;
			ex.im = 0; ey.im = 0; ez.im = 0;


			double a = K_0 * sin(thetaGlobal * pi180) * cos(phiGlobal * pi180);
			double b = K_0 * sin(thetaGlobal * pi180) * sin(phiGlobal * pi180);
			double c = K_0 * cos(thetaGlobal * pi180);

			double ab = a * b;
			double bc = b * c;
			double ac = a * c;

			for (int k = 0; k < appSize; k++)
			{
				double arg = a * trX[k] + b * trY[k] + c * trZ[k];
				CComplex cexp;
				cexp.re = cos(arg);
				cexp.im = sin(arg);

				sol[3 * j + 0] = sol[3 * j + 0] - Ekoeff * (K2 * Ix[k] - a * a * Ix[k] - ab * Iy[k] - ac * Iz[k]) * cexp * trS[k] - imOne * (b * Mz[k] - c * My[k]) * cexp * trS[k] / (4 * pi);//-
				sol[3 * j + 1] = sol[3 * j + 1] - Ekoeff * (K2 * Iy[k] - b * b * Iy[k] - bc * Iz[k] - ab * Ix[k]) * cexp * trS[k] - imOne * (c * Mx[k] - a * Mz[k]) * cexp * trS[k] / (4 * pi);//-
				sol[3 * j + 2] = sol[3 * j + 2] - Ekoeff * (K2 * Iz[k] - c * c * Iz[k] - ac * Ix[k] - bc * Iy[k]) * cexp * trS[k] - imOne * (a * My[k] - b * Mx[k]) * cexp * trS[k] / (4 * pi);//-
			}
		}
	}
	return sol;
}




extern "C" __declspec(dllexport) 
CComplex* ApertureRadomeFF(CComplex* Ix, CComplex* Iy, CComplex* Iz, CComplex* Mx, CComplex* My, CComplex* Mz,
	double* trX, double* trY, double* trZ, double* trS, double* Nx, double* Ny, double* Nz, int geoSize,
	CComplex* eps, CComplex* mu, double* tickness, double* gtickness, int* layersCount, int* layersIndexer,
	double freq, double thetaStart, double thetaFinish, double phiStart, double phiFinish, double thetaStep, double phiStep,
	int systemOfCoord, int proc)
{
#ifndef _OPENMP
	std::cout << "OpenMP is not supported!\n";
	return nullptr;
#endif	

	//установка числа потоков
	if (omp_get_max_threads() < proc)
	{
		proc = omp_get_max_threads();
	}
	omp_set_num_threads(proc);

	//Определение физических констант	
	const double Omega = 2 * pi * freq;
	const double Omega2 = Omega * Omega;
	const double K_0 = Omega / с0;
	const double K2 = K_0 * K_0;
	CComplex imOne;
	imOne.re = 0,
		imOne.im = 1;
	const CComplex iOmega = imOne * Omega;
	const CComplex Ekoeff = (1.0) / (4.0 * pi * imOne * Omega * E_0); //fourComx*piComx*imOne*omegaComx*E_0Comx                            
	const double pi4 = 4.0 * pi;
	const double pi180 = pi / 180.0;
	const double pi_2 = pi / 2;
	const CComplex Z0m = imOne * Omega * Mu_0;
	const CComplex Y0e = imOne * Omega * E_0;

	//Максимально допустимый угол 
	double cutoff_angle = 88.0 * pi180;                                   //  в радианах
	//Расчёт
	int numberPhi = roundInt((phiFinish - phiStart) / phiStep) + 1;
	int numberTheta = roundInt((thetaFinish - thetaStart) / thetaStep) + 1;
	int numberTotal = numberPhi * numberTheta;
	int counterTotal = 3 * numberTotal;
	CComplex* sol = new CComplex[counterTotal];
	for (int i = 0; i < counterTotal; i++)
	{
		sol[i].re = 0;
		sol[i].im = 0;
	}
	myVector ran1;
	ran1.x = 0;
	ran1.y = 0;
	ran1.z = 1;
	myVector ran2;
	ran2.x = 0;
	ran2.y = 1;
	ran2.z = 0;

	bool writetrigger = false;
#pragma omp parallel shared (Ix, Iy, Iz, Mx, My, Mz, trX, trY, trZ, trS, Nx, Ny, Nz, geoSize, eps, mu, tickness, layersCount, gtickness, layersIndexer, freq, thetaStart, thetaFinish, phiStart, phiFinish, thetaStep, phiStep, systemOfCoord, proc, sol)
	{	
		int cur_proc = omp_get_thread_num();	//идентификация текущего потока
		int start = GetStartIndex(cur_proc, proc, numberTotal);
		int end = GetEndIndex(cur_proc, proc, numberTotal);

		//Номер направления расчета
		for (int j = start; j <= end; j++)
		{
			double phi = phiStart + (j % numberPhi) * phiStep;
			double theta = thetaStart + j / numberPhi * thetaStep;

			double thetaGlobal = GetThetaGlobal(phi, theta, systemOfCoord);	//  в радианах
			double phiGlobal = GetPhiGlobal(phi, theta, systemOfCoord);

			double thetaGlobalRad = thetaGlobal * pi180;      //  в радианах
			double phiGlobalRad = phiGlobal * pi180;          //  в радианах

			myVector loc1, loc2;
			//
			// расчет направление вектора распространения волны
			//
			myVector k;
			k.x = sin(thetaGlobalRad) * cos(phiGlobalRad);  // вектор k = {kx, ky, kz}
			k.y = sin(thetaGlobalRad) * sin(phiGlobalRad);  // направление распространиения
			k.z = cos(thetaGlobalRad);                             // волны в глобальных координатах
			myVector k_loc;

			//Номер элемента геометрии
			for (int i = 0; i < geoSize; i++)
			{
				//
				//   выбор произвольного вектора
				//				
				myVector n;
				n.x = Nx[i]; n.y = Ny[i]; n.z = Nz[i];

				loc1 = Cross(n, ran1);

				if (loc1.Modulus() < 1.0e-8)
				{
					loc1 = Cross(n, ran2);
				}

				myVector temp = Normalize(loc1);
				loc1 = temp;

				//
				//   Определение осей локальной системы координат
				//   
				loc2 = Cross(n, loc1);
				temp = Normalize(loc2);
				loc2 = temp;

				double scalNK = Scal(n, k);     // скалярное произведение вектора нормали n и вектора k;
				if (scalNK > 1)
				{
					scalNK = 1;
				}
				if (scalNK < -1)
				{
					scalNK = -1;
				}

				//
				// Определение локальных сферических координат
				//    
				double thetaLoc = acos(scalNK);                // угол theta в локальной системе координат, в радианах          
				if (scalNK < 0)
				{
					loc2 = (-1) * loc2;
					n.x = (-1) * n.x;
					n.y = (-1) * n.y;
					n.z = (-1) * n.z;
					thetaLoc = pi - acos(scalNK);                               // угол theta в локальной системе координат, в радианах
				}

				if (thetaLoc > cutoff_angle && thetaLoc <= pi_2)
				{
					thetaLoc = cutoff_angle;
				}
				else if (thetaLoc < 0.001)
				{
					thetaLoc = 0.001;
				}

				//
				// расчет каппа 1 и каппа 2
				//
				k_loc = k - cos(thetaLoc) * n;
				temp = Normalize(k_loc);
				k_loc = temp;

				double scalPLoc1 = Scal(k_loc, loc1);           // скалярное произведение вектора p и вектора loc1
				
				if (scalPLoc1 > 1)
				{
					scalPLoc1 = 1;
				}
				if (scalPLoc1 < -1)
				{
					scalPLoc1 = -1;
				}
				double scalPLoc2 = Scal(k_loc, loc2);       // скалярное произведение вектора p и вектора loc2

				if (scalPLoc2 > 1)
				{
					scalPLoc2 = 1;
				}
				if (scalPLoc2 < -1)
				{
					scalPLoc2 = -1;
				}

				double phiLoc = 0;

				if (scalPLoc2 >= 0)
				{
					phiLoc = acos(scalPLoc1);          // угол phi  в локальной системе координат, в радианах
				}
				else
				{
					phiLoc = (-1) * acos(scalPLoc1);       // угол phi в локальной системе координат, в радианах
				}

				double kp1 = K_0 * sin(thetaLoc) * cos(phiLoc);   // kp1, kp2 - пространственные частоты
				double kp2 = K_0 * sin(thetaLoc) * sin(phiLoc);   // или период изменения фазы вдоль выделенноего направления (углы в радианах)

				double kappa2 = kp1 * kp1 + kp2 * kp2;
				CComplex ikappa2 = imOne * kappa2;
				double kp1dS = kp1 * trS[i];
				double kp2dS = kp2 * trS[i];
				
				//
				// Расчет локальных токов
				//    
				myVectorC jc;
				jc.x = Ix[i];
				jc.y = Iy[i];
				jc.z = Iz[i];

				myVectorC mc;
				mc.x = Mx[i];
				mc.y = My[i];
				mc.z = Mz[i];

				CComplex iLoc1, iLoc2;      // локальные электрические токи								
				iLoc1 = ScalC(loc1, jc);
				iLoc2 = ScalC(loc2, jc);

				CComplex mLoc1, mLoc2;      // локальные электрические токи								
				mLoc1 = ScalC(loc1, mc);
				mLoc2 = ScalC(loc2, mc);

				//
				// расчет гамма в свободном пространстве
				//               
				CComplex gamma0 = Csqrt(kappa2 - K2);
				//
				//   Волновое сопротивление в свободном пространстве
				//

				CComplex W0e = gamma0 / Y0e;
				CComplex W0m = Z0m / gamma0;


				//////////////////////////////////////////////////////////////////////////////////
				///////////Определение коэффициентов прохождения//////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////


				//
				// матрица передачи диэлектрического слоя
				//
				CComplex mEtype[2][2], mMtype[2][2];
				CComplex one, zer;
				one.re = 1; one.im = 0;
				zer.re = 0; zer.im = 0;

				mEtype[0][0] = one; mEtype[0][1] = zer; mEtype[1][0] = zer; mEtype[1][1] = one;
				mMtype[0][0] = one; mMtype[0][1] = zer; mMtype[1][0] = zer; mMtype[1][1] = one;

				int indexOfStenka = layersIndexer[i];
				int layerCount = layersCount[indexOfStenka];
				double generalTickness = gtickness[indexOfStenka];
				int numberoflayersbefore = 0;
				for (int q = 0; q < indexOfStenka; q++)
				{
					numberoflayersbefore += layersCount[q];
				}

				//Номер слоя
				for (int t = 0; t < layerCount; t++)
				{

					CComplex Ea = eps[numberoflayersbefore + t];
					CComplex Mua = mu[numberoflayersbefore + t];
					double d = tickness[numberoflayersbefore + t];
					CComplex Zm = iOmega * Mua;
					CComplex Ye = iOmega * Ea;

					CComplex a_e, b_e, c_e, d_e;
					CComplex a_m, b_m, c_m, d_m;


					//
					// расчет гамма в диэлектрике и в свободном пространстве
					//
					CComplex gamma = Csqrt(kappa2 - Omega2 * Mua * Ea);
					//
					//   Волновое сопротивление в диэлектрике
					//
					CComplex we = gamma / Ye;
					CComplex wm = Zm / gamma;
					CComplex gammaL = gamma * d;


					CComplex sinhGT = Sinh(gammaL);
					CComplex coshGT = Cosh(gammaL);



					a_e = coshGT;
					a_m = coshGT;
					b_e = we * sinhGT;
					b_m = wm * sinhGT;
					c_e = 1 / we * sinhGT;
					c_m = 1 / wm * sinhGT;
					d_e = coshGT;
					d_m = coshGT;


					CComplex kmEtype[2][2], kmMtype[2][2];
					kmEtype[0][0] = a_e;
					kmEtype[0][1] = b_e;
					kmEtype[1][0] = c_e;
					kmEtype[1][1] = d_e;

					kmMtype[0][0] = a_m;
					kmMtype[0][1] = b_m;
					kmMtype[1][0] = c_m;
					kmMtype[1][1] = d_m;

					CComplex tempE[2][2];
					tempE[0][0] = mEtype[0][0] * kmEtype[0][0] + mEtype[0][1] * kmEtype[1][0];
					tempE[0][1] = mEtype[0][0] * kmEtype[0][1] + mEtype[0][1] * kmEtype[1][1];
					tempE[1][0] = mEtype[1][0] * kmEtype[0][0] + mEtype[1][1] * kmEtype[1][0];
					tempE[1][1] = mEtype[1][0] * kmEtype[0][1] + mEtype[1][1] * kmEtype[1][1];

					CComplex tempM[2][2];
					tempM[0][0] = mMtype[0][0] * kmMtype[0][0] + mMtype[0][1] * kmMtype[1][0];
					tempM[0][1] = mMtype[0][0] * kmMtype[0][1] + mMtype[0][1] * kmMtype[1][1];
					tempM[1][0] = mMtype[1][0] * kmMtype[0][0] + mMtype[1][1] * kmMtype[1][0];
					tempM[1][1] = mMtype[1][0] * kmMtype[0][1] + mMtype[1][1] * kmMtype[1][1];

					for (int ks = 0; ks < 2; ks++)
					{
						for (int ls = 0; ls < 2; ls++)
						{
							mEtype[ks][ls] = tempE[ks][ls];
							mMtype[ks][ls] = tempM[ks][ls];
						}
					}
				}

				////
				////   Волновое сопротивление в свободном пространстве
				////
				//CComplex w0e = gamma0 / Y0e;
				//CComplex w0m = Z0m / gamma0;


				CComplex A_e = mEtype[0][0];    // а элемент матрицы передачи сквозь весь диэлектрик для полей типа Е
				CComplex A_m = mMtype[0][0];    // а элемент матрицы передачи сквозь весь диэлектрик для полей типа H
				CComplex B_e = mEtype[0][1];    // b элемент матрицы передачи сквозь весь диэлектрик для полей типа Е
				CComplex B_m = mMtype[0][1];    // b элемент матрицы передачи сквозь весь диэлектрик для полей типа H
				CComplex C_e = mEtype[1][0];    // с элемент матрицы передачи сквозь весь диэлектрик для полей типа Е
				CComplex C_m = mMtype[1][0];    // с элемент матрицы передачи сквозь весь диэлектрик для полей типа H
				CComplex D_e = mEtype[1][1];    // d элемент матрицы передачи сквозь весь диэлектрик для полей типа Е
				CComplex D_m = mMtype[1][1];    // d элемент матрицы передачи сквозь весь диэлектрик для полей типа H
				//
				//   входное сопротивление длинной линии со стороны слоя
				//
				/*CComplex inputZe = (A_e * W0e + B_e) / (C_e * W0e + D_e);
				CComplex inputZm = (A_m * W0m + B_m) / (C_m * W0m + D_m);*/
				//
				// расчет коэффициента прохождения
				//
				CComplex transitionE;
				CComplex transitionH;

				transitionE = 1 / (A_e + B_e / W0e + C_e * W0e + D_e);
				transitionH = 1 / (A_m + B_m / W0m + C_m * W0m + D_m);
												
				//////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////
				//////////////////////////////////////////////////////////////////////////////////

				//
				//   Расчет спектров входных источников напряжения и токов
				//
				CComplex spectorFe = (kp1dS * iLoc1 + kp2dS * iLoc2) / ikappa2;
				CComplex spectorFm = (kp2dS * iLoc1 - kp1dS * iLoc2) / ikappa2;
				CComplex spectorEe = (-1) * (kp2dS * mLoc1 - kp1dS * mLoc2) / ikappa2;
				CComplex spectorEm = (kp1dS * mLoc1 + kp2dS * mLoc2) / ikappa2;
				//
				// Входные источники напряжения электрического и магнитного полей
				//
				CComplex ue1 = (spectorEe + spectorFe * W0e);            //!!!!!!!!!!!!!!!!!!!!!!!!!! поделил на 2 как в формуле
				CComplex um1 = (spectorEm + spectorFm * W0m);
				//
				// Выходные источники напряжения электрического и магнитного полей
				//
				CComplex ue2 = ue1 * transitionE;
				CComplex um2 = um1 * transitionH;
				//
				// расчет спектра полей
				//
				CComplex spExLoc, spEyLoc, spEzLoc;       // x, y, z компоненты спектора 
				//spExLoc.re = 0, spExLoc.im = 0;           // электричекого поля в локальной 
				//spEyLoc.re = 0, spEyLoc.im = 0;           // системе координат   
				//spEzLoc.re = 0, spEzLoc.im = 0;

				spExLoc = (-1) * imOne * (kp1 * ue2 + kp2 * um2);
				spEyLoc = (-1) * imOne * (kp2 * ue2 - kp1 * um2);
				spEzLoc = (-1) * kappa2 * ue2 / gamma0;

				CComplex spEx, spEy, spEz;	            //x, y, z компоненты спектора 
				//spEx.re = 0; spEx.im = 0;               //электричекого поля в глобальной 
				//spEy.re = 0; spEy.im = 0;				//системе координат
				//spEz.re = 0; spEz.im = 0;
				spEx = (spExLoc * loc1.x + spEyLoc * loc2.x + spEzLoc * n.x) / pi4;          //
				spEy = (spExLoc * loc1.y + spEyLoc * loc2.y + spEzLoc * n.y) / pi4;          // домножил на i*omega*E_0/4/pi
				spEz = (spExLoc * loc1.z + spEyLoc * loc2.z + spEzLoc * n.z) / pi4;          // 
				//
				// расчет поля в дальней зоне
				//
				double gammaLoc = 0;
				if (thetaLoc >= pi / 2)
				{
					gammaLoc = K_0 * cos(pi - thetaLoc);
				}
				else
				{
					gammaLoc = K_0 * cos(thetaLoc);
				}


				CComplex i2gammaLoc = 2 * gammaLoc * imOne;


				double argument = K_0 * (trX[i] * k.x + trY[i] * k.y + trZ[i] * k.z) + generalTickness * gammaLoc;
				CComplex exp;
				exp.re = cos(argument);
				exp.im = sin(argument);				

				sol[3 * j + 0] = sol[3 * j + 0] - i2gammaLoc * spEx * exp;
				sol[3 * j + 1] = sol[3 * j + 1] - i2gammaLoc * spEy * exp;
				sol[3 * j + 2] = sol[3 * j + 2] - i2gammaLoc * spEz * exp;				
			}
		}
	}

	return sol;
}

extern "C" __declspec(dllexport)
CComplex* ReflactedFromRadomeNF(CComplex* incEx, CComplex* incEy, CComplex* incEz, CComplex* incHx, CComplex* incHy, CComplex* incHz, 
								double* Nx, double* Ny, double* Nz, double* px, double* py, double* pz, 
								CComplex* eps_a, CComplex* mu_a, double* tickness, double* gtickness, int* layersCount, int* stenkaIndexer,
								int numberElements, double freq, int &proc)
{
#ifndef _OPENMP
	std::cout << "OpenMP is not supported!\n";
	return nullptr;
#endif

	//В С# возвращается "спресованный" одномерный массив, где на позициях {0,1,2} - Ex, Ey, Ez; {3,4,5} - Hx, Hy, Hz;
	int totalNumber = numberElements * 6;
	CComplex* solution = new CComplex[totalNumber];

	for (int i = 0; i < totalNumber; i++)
	{
		solution[i].re = 0;
		solution[i].im = 0;
	}
	//установка числа потоков
	if (omp_get_max_threads() < proc)
	{
		proc = omp_get_max_threads();
	}
	omp_set_num_threads(proc);


	static CComplex imOne;
	imOne.re = 0;
	imOne.im = 1;

	double Lambda = с0 / freq;
	double Omega = 2 * pi * freq;
	const double Omega2 = Omega * Omega;
	const CComplex iOmega = imOne * Omega;
	double K_0 = 2 * pi / Lambda;      // волновое число 2pi/lambda
	double K2 = K_0 * K_0;
	CComplex iomegaE = imOne * Omega * E_0;
	CComplex iomegaMu = imOne * Omega * Mu_0;

	CComplex Z0m = imOne * Omega * Mu_0;
	CComplex Y0e = imOne * Omega * E_0;

	CComplex Ekoeff = (-1.0) / iomegaE;          // 1/i*omega*E_0
	CComplex Mukoeff = (-1.0) / iomegaMu;         // -1/i*omega*Mu_0
	//Максимально допустимый угол падения
	double cutoff_angle = 88.0 * pi / 180;                                   // в радианах 88 градусов


#pragma omp parallel shared (K_0, K2, imOne, Ekoeff, Mukoeff, cutoff_angle, incEx, incEy, incEz, incHx, incHy, incHz, px, py, pz, Nx, Ny, Nz, freq, numberElements, eps_a, mu_a, tickness, gtickness, layersCount, stenkaIndexer)
	{
		int proc = omp_get_num_threads();		//количество потоков
		int cur_proc = omp_get_thread_num();	//идентификация текущего потока

		int start = GetStartIndex(cur_proc, proc, numberElements);
		int end = GetEndIndex(cur_proc, proc, numberElements);


		for (int j = start; j <= end; j++)
		{
			double thetaLoc = 0;

			double nx = Nx[j];
			double ny = Ny[j];
			double nz = Nz[j];

			CComplex Eincx = incEx[j];
			CComplex Eincy = incEy[j];
			CComplex Eincz = incEz[j];

			CComplex Hincx = incHx[j];
			CComplex Hincy = incHy[j];
			CComplex Hincz = incHz[j];

			CComplex HincxCoj = Conjugate(incHx[j]);
			CComplex HincyCoj = Conjugate(incHy[j]);
			CComplex HinczCoj = Conjugate(incHz[j]);

			CComplex JincComplexx = Eincy * HinczCoj - Eincz * HincyCoj;
			CComplex JincComplexy = Eincz * HincxCoj - Eincx * HinczCoj;
			CComplex JincComplexz = Eincx * HincyCoj - Eincy * HincxCoj;

			double kx = JincComplexx.re;
			double ky = JincComplexy.re;
			double kz = JincComplexz.re;

			double modulus = sqrt(kx * kx + ky * ky + kz * kz);
			kx /= modulus;
			ky /= modulus;
			kz /= modulus;

			double scalNK = kx * nx + ky * ny + kz * nz;                        // скалярное произведение вектора нормали n и вектора k;                        
			//
			// Определение thetaLoc
			//    
			if (abs(scalNK) >= 1)
			{
				thetaLoc = 0;
			}
			else if (abs(scalNK) >= 0.035)
			{
				thetaLoc = acos(scalNK);                // угол theta в локальной системе координат, в радианах      
			}
			else if (abs(scalNK) >= 0)
			{
				thetaLoc = cutoff_angle;
			}

			double VincRx, VincRy, VincRz;
			if (thetaLoc > 0.001)
			{
				VincRx = ky * nz - kz * ny;
				VincRy = kz * nx - kx * nz;
				VincRz = kx * ny - ky * nx;
				double modulus2 = sqrt(VincRx * VincRx + VincRy * VincRy + VincRz * VincRz);
				VincRx /= modulus2;
				VincRy /= modulus2;
				VincRz /= modulus2;
			}
			else
			{
				if (abs(nx) < 0.98)
				{
					VincRx = 0;
					VincRy = nz;
					VincRz = (-1) * ny;
					double modulus2 = sqrt(VincRy * VincRy + VincRz * VincRz);

					VincRy /= modulus2;
					VincRz /= modulus2;
				}
				else
				{
					VincRx = (-1) * nz;
					VincRy = 0;
					VincRz = nx;
					double modulus2 = sqrt(VincRx * VincRx + VincRz * VincRz);
					VincRx /= modulus2;

					VincRz /= modulus2;
				}
			}
			double VincLx = ky * VincRz - kz * VincRy;
			double VincLy = kz * VincRx - kx * VincRz;
			double VincLz = kx * VincRy - ky * VincRx;

			double modulus3 = sqrt(VincLx * VincLx + VincLy * VincLy + VincLz * VincLz);
			VincLx /= modulus3;
			VincLy /= modulus3;
			VincLz /= modulus3;

			double kp1 = K_0 * sin(thetaLoc);   // kp1, kp2 - пространственные частоты             


			double kappa2 = kp1 * kp1;
			//Stenka st = radUnion[stenkaIndexer[j]].Structure;
			//Tuple<Complex, Complex, Complex, Complex> reflect = TRCoeffitients.ReflectionCoefficientCalc2(st, kappa2, thetaLoc, Omega, K2, Y0e, Z0m);

			//////////////////////////////////////////////////////////////////////////////////
			///////////Определение коэффициентов прохождения//////////////////////////////////
			//////////////////////////////////////////////////////////////////////////////////


			//
			// матрица передачи диэлектрического слоя
			//
			CComplex mEtype[2][2], mMtype[2][2];
			CComplex one, zer;
			one.re = 1; one.im = 0;
			zer.re = 0; zer.im = 0;

			mEtype[0][0] = one; mEtype[0][1] = zer; mEtype[1][0] = zer; mEtype[1][1] = one;
			mMtype[0][0] = one; mMtype[0][1] = zer; mMtype[1][0] = zer; mMtype[1][1] = one;

			int indexOfStenka = stenkaIndexer[j];
			int layerCount = layersCount[indexOfStenka];
			double generalTickness = gtickness[indexOfStenka];
			int numberoflayersbefore = 0;
			for (int q = 0; q < indexOfStenka; q++)
			{
				numberoflayersbefore += layersCount[q];
			}

			//Номер слоя
			for (int t = 0; t < layerCount; t++)
			{

				CComplex Ea = eps_a[numberoflayersbefore + t];
				CComplex Mua = mu_a[numberoflayersbefore + t];
				double d = tickness[numberoflayersbefore + t];
				CComplex Zm = iOmega * Mua;
				CComplex Ye = iOmega * Ea;

				CComplex a_e, b_e, c_e, d_e;
				CComplex a_m, b_m, c_m, d_m;


				//
				// расчет гамма в диэлектрике и в свободном пространстве
				//
				CComplex gamma = Csqrt(kappa2 - Omega2 * Mua * Ea);
				//
				//   Волновое сопротивление в диэлектрике
				//
				CComplex we = gamma / Ye;
				CComplex wm = Zm / gamma;
				CComplex gammaL = gamma * d;


				CComplex sinhGT = Sinh(gammaL);
				CComplex coshGT = Cosh(gammaL);



				a_e = coshGT;
				a_m = coshGT;
				b_e = we * sinhGT;
				b_m = wm * sinhGT;
				c_e = 1 / we * sinhGT;
				c_m = 1 / wm * sinhGT;
				d_e = coshGT;
				d_m = coshGT;


				CComplex kmEtype[2][2], kmMtype[2][2];
				kmEtype[0][0] = a_e;
				kmEtype[0][1] = b_e;
				kmEtype[1][0] = c_e;
				kmEtype[1][1] = d_e;

				kmMtype[0][0] = a_m;
				kmMtype[0][1] = b_m;
				kmMtype[1][0] = c_m;
				kmMtype[1][1] = d_m;

				CComplex tempE[2][2];
				tempE[0][0] = mEtype[0][0] * kmEtype[0][0] + mEtype[0][1] * kmEtype[1][0];
				tempE[0][1] = mEtype[0][0] * kmEtype[0][1] + mEtype[0][1] * kmEtype[1][1];
				tempE[1][0] = mEtype[1][0] * kmEtype[0][0] + mEtype[1][1] * kmEtype[1][0];
				tempE[1][1] = mEtype[1][0] * kmEtype[0][1] + mEtype[1][1] * kmEtype[1][1];

				CComplex tempM[2][2];
				tempM[0][0] = mMtype[0][0] * kmMtype[0][0] + mMtype[0][1] * kmMtype[1][0];
				tempM[0][1] = mMtype[0][0] * kmMtype[0][1] + mMtype[0][1] * kmMtype[1][1];
				tempM[1][0] = mMtype[1][0] * kmMtype[0][0] + mMtype[1][1] * kmMtype[1][0];
				tempM[1][1] = mMtype[1][0] * kmMtype[0][1] + mMtype[1][1] * kmMtype[1][1];

				for (int ks = 0; ks < 2; ks++)
				{
					for (int ls = 0; ls < 2; ls++)
					{
						mEtype[ks][ls] = tempE[ks][ls];
						mMtype[ks][ls] = tempM[ks][ls];
					}
				}
			}			
			//
			// расчет гамма в свободном пространстве
			//               
			CComplex gamma0 = Csqrt(kappa2 - K2);			
			//
			//   Волновое сопротивление в свободном пространстве
			//
			CComplex w0e = gamma0 / Y0e;
			CComplex w0m = Z0m / gamma0;


			CComplex A_e = mEtype[0][0];    // а элемент матрицы передачи сквозь весь диэлектрик для полей типа Е
			CComplex A_m = mMtype[0][0];    // а элемент матрицы передачи сквозь весь диэлектрик для полей типа H
			CComplex B_e = mEtype[0][1];    // b элемент матрицы передачи сквозь весь диэлектрик для полей типа Е
			CComplex B_m = mMtype[0][1];    // b элемент матрицы передачи сквозь весь диэлектрик для полей типа H
			CComplex C_e = mEtype[1][0];    // с элемент матрицы передачи сквозь весь диэлектрик для полей типа Е
			CComplex C_m = mMtype[1][0];    // с элемент матрицы передачи сквозь весь диэлектрик для полей типа H
			CComplex D_e = mEtype[1][1];    // d элемент матрицы передачи сквозь весь диэлектрик для полей типа Е
			CComplex D_m = mMtype[1][1];    // d элемент матрицы передачи сквозь весь диэлектрик для полей типа H
			//
			//   входное сопротивление длинной линии со стороны слоя
			//
			
			CComplex inputZe = (A_e * w0e + B_e) / (C_e * w0e + D_e);
			CComplex inputZm = (A_m * w0m + B_m) / (C_m * w0m + D_m);

			CComplex refE = (inputZe - w0e) / (inputZe + w0e);
			CComplex refH = (inputZm - w0m) / (inputZm + w0m);;
			/*CComplex refE = B_e + A_e * w0e - D_e * w0e - C_e * w0e*w0e;
			CComplex refH = B_m + A_m * w0m - D_e * w0m - C_m * w0m*w0m;*/
			
			

			CComplex EincL = Eincx * VincLx + Eincy * VincLy + Eincz * VincLz;
			CComplex EincR = Eincx * VincRx + Eincy * VincRy + Eincz * VincRz;

			CComplex HincL = Hincx * VincLx + Hincy * VincLy + Hincz * VincLz;
			CComplex HincR = Hincx * VincRx + Hincy * VincRy + Hincz * VincRz;


			double VrefRx = VincRx;
			double VrefRy = VincRy;
			double VrefRz = VincRz;

			//double doubleVincLByNScal = Math.Abs(VincLx * nx + VincLy * ny + VincLz * nz);                        
			double kref_x = kx - 2 * nx * cos(thetaLoc);
			double kref_y = ky - 2 * ny * cos(thetaLoc);
			double kref_z = kz - 2 * nz * cos(thetaLoc);			

			double VrefLx = kref_z * VincRy - kref_y * VincRz;
			double VrefLy = kref_x * VincRz - kref_z * VincRx;
			double VrefLz = kref_y * VincRx - kref_x * VincRy;

			double modulus4 = sqrt(VrefLx * VrefLx + VrefLy * VrefLy + VrefLz * VrefLz);
			VrefLx /= modulus4;
			VrefLy /= modulus4;
			VrefLz /= modulus4;

			CComplex ErefLx = refE * EincL * VrefLx; //(2 * (SCVector.Scal(EincL, n) * n) - EincL)
			CComplex ErefLy = refE * EincL * VrefLy;
			CComplex ErefLz = refE * EincL * VrefLz;

			CComplex ErefRx = refH * EincR * VrefRx;
			CComplex ErefRy = refH * EincR * VrefRy;
			CComplex ErefRz = refH * EincR * VrefRz;

			CComplex HrefLx = refH * HincL * VrefLx;
			CComplex HrefLy = refH * HincL * VrefLy;
			CComplex HrefLz = refH * HincL * VrefLz;

			CComplex HrefRx = refE * HincR * VrefRx;
			CComplex HrefRy = refE * HincR * VrefRy;
			CComplex HrefRz = refE * HincR * VrefRz;


			CComplex Erefx = (-1)*(ErefLx + ErefRx);
			CComplex Erefy = (-1)*(ErefLy + ErefRy);
			CComplex Erefz = (-1)*(ErefLz + ErefRz);

			CComplex Hrefx = (HrefLx + HrefRx);
			CComplex Hrefy = (HrefLy + HrefRy);
			CComplex Hrefz = (HrefLz + HrefRz);			

			solution[6 * j + 0] = Erefx;
			solution[6 * j + 1] = Erefy;
			solution[6 * j + 2] = Erefz;
			solution[6 * j + 3] = Hrefx;
			solution[6 * j + 4] = Hrefy;
			solution[6 * j + 5] = Hrefz;			
		}
	}
	return solution;
}


extern "C" __declspec(dllexport)
CComplex* InducedNF(CComplex* Ix, CComplex* Iy, CComplex* Iz, CComplex* Mx, CComplex* My, CComplex* Mz,
double* Nx, double* Ny, double* Nz, double* Area, int size,
double* pXO, double* pYO, double* pZO,
double* pXC, double* pYC, double* pZC,
double* pX1, double* pY1, double* pZ1,
double* pX2, double* pY2, double* pZ2,
double* pX3, double* pY3, double* pZ3,
double freq, int& pthread)
{
	//В С# передаётся "спресованный" одномерный массив, где на позициях {0,1,2} - Ex, Ey, Ez; {3,4,5} - Hx, Hy, Hz;
	int totalNumber = size * 6;
	CComplex* solution = new CComplex[totalNumber];

	for (int i = 0; i < totalNumber; i++)
	{
		solution[i].re = 0;
		solution[i].im = 0;
	}

	//установка числа потоков
	if (omp_get_max_threads() < pthread)
	{
		pthread = omp_get_max_threads();
	}
	omp_set_num_threads(pthread);

	static CComplex imOne;
	imOne.re = 0;
	imOne.im = 1;
	double Lambda = с0 / freq;
	double Omega = 2 * pi * freq;
	double K_0 = 2 * pi / Lambda;      // волновое число 2pi/lambda
	double K2 = K_0 * K_0;
	CComplex iomegaE = imOne * Omega * E_0;
	CComplex iomegaMu = imOne * Omega * Mu_0;

	CComplex Ekoeff = (-1.0) / iomegaE;          // 1/i*omega*E_0
	CComplex Mukoeff = (-1.0) / iomegaMu;         // -1/i*omega*Mu_0

#pragma omp parallel shared (K_0, K2, imOne, Ekoeff, Mukoeff, pXO, pYO, pZO, pXC, pYC, pZC , pX1, pY1, pZ1, pX2, pY2, pZ2, pX3, pY3, pZ3, Area, Ix, Iy, Iz, Mx, My, Mz, Nx, Ny, Nz, freq, size)
	{		
		///
		int proc = omp_get_num_threads();		//количество потоков
		int cur_proc = omp_get_thread_num();	//идентификация текущего потока

		int start = GetStartIndex(cur_proc, proc, size);
		int end = GetEndIndex(cur_proc, proc, size);

		for (int j = start; j < end; j++)
		{
			CComplex ex, ey, ez, hx, hy, hz;
			ex.re = 0;
			ey.re = 0;
			ez.re = 0;
			hx.re = 0;
			hy.re = 0;
			hz.re = 0;

			ex.im = 0;
			ey.im = 0;
			ez.im = 0;
			hx.im = 0;
			hy.im = 0;
			hz.im = 0;

			for (int i = 0; i < size; i++)
			{
				double dS = Area[i];

				CComplex ix = Ix[i];
				CComplex iy = Ix[i];
				CComplex iz = Ix[i];

				CComplex mx = Mx[i];
				CComplex my = My[i];
				CComplex mz = Mz[i];


				double x_x0 = pXC[i] - pXO[j];
				double y_y0 = pYC[i] - pYO[j];
				double z_z0 = pZC[i] - pZO[j];
				double x2 = x_x0 * x_x0;
				double y2 = y_y0 * y_y0;
				double z2 = z_z0 * z_z0;

				double r = sqrt(x2 + y2 + z2);

				if (r < 0.09f)//r < 0.03f
				{
					double xq, yq, zq;
					//
					//   Расчет при условии близкого расположения точки интергрирования и точки наблюдения по правилу 4-х точечной квадратуры
					//
					//Точка 1
					float a = 0.33333333f;      //
					float b = 0.33333333f;      // координаты
					float g = 0.33333333f;      //
					float w = -0.56250000f;     // вес

					xq = g * pX1[i] + a * pX2[i] + b * pX3[i];
					yq = g * pY1[i] + a * pY2[i] + b * pY3[i];
					zq = g * pZ1[i] + a * pZ2[i] + b * pZ3[i];

					x_x0 = xq - pXO[j];
					y_y0 = yq - pYO[j];
					z_z0 = zq - pZO[j];

					x2 = x_x0 * x_x0;
					y2 = y_y0 * y_y0;
					z2 = z_z0 * z_z0;

					r = sqrt(x2 + y2 + z2);

					CComplex exp_ikr;
					exp_ikr.re = cos(K_0 * r);
					exp_ikr.im = (-1)*sin(K_0 * r);

					CComplex funG = exp_ikr / (4 * pi * r);
					double r2 = r * r;
					double r4 = r2 * r2;

					//
					//НАЧАЛО КОСТИНЫ ФОРМУЛЫ
					//
					CComplex coeffA = (-1) * imOne * K_0 / r - 1.0 / r2;
					CComplex coeffB = (3.0 + 3.0 * imOne * K_0 * r - K2 * r2) / r4;

					CComplex dx = x_x0 * coeffA * funG;
					CComplex dy = y_y0 * coeffA * funG;
					CComplex dz = z_z0 * coeffA * funG;

					CComplex dxx = (x2 * coeffB + coeffA) * funG;
					CComplex dyy = (y2 * coeffB + coeffA) * funG;
					CComplex dzz = (z2 * coeffB + coeffA) * funG;

					CComplex dxy = (x_x0 * y_y0 * coeffB) * funG;
					CComplex dxz = (x_x0 * z_z0 * coeffB) * funG;
					CComplex dyz = (y_y0 * z_z0 * coeffB) * funG;
					//
					// КОНЕЦ
					//					
					CComplex exc = Ekoeff * (K2 * ix * funG + ix * dxx + iy * dxy + iz * dxz) * dS - (mz * dy - my * dz) * dS; //-
					CComplex eyc = Ekoeff * (K2 * iy * funG + iy * dyy + iz * dyz + ix * dxy) * dS - (mx * dz - mz * dx) * dS; //-
					CComplex ezc = Ekoeff * (K2 * iz * funG + iz * dzz + ix * dxz + iy * dyz) * dS - (my * dx - mx * dy) * dS; //-

					CComplex hxc = Mukoeff * (K2 * mx * funG + mx * dxx + my * dxy + mz * dxz) * dS + (iz * dy - iy * dz) * dS; //+
					CComplex hyc = Mukoeff * (K2 * my * funG + mx * dxy + my * dyy + mz * dyz) * dS + (ix * dz - iz * dx) * dS; //+
					CComplex hzc = Mukoeff * (K2 * mz * funG + mx * dxz + my * dyz + mz * dzz) * dS + (iy * dx - ix * dy) * dS; //+					

					ex = ex + w * exc;
					ey = ey + w * eyc;
					ez = ez + w * ezc;

					hx = hx + w * hxc;
					hy = hy + w * hyc;
					hz = hz + w * hzc;

					//Точка 2
					a = 0.60000000f;        //
					b = 0.20000000f;        // координаты
					g = 0.20000000f;        //
					w = 0.52083333f;       // вес

					xq = g * pX1[i] + a * pX2[i] + b * pX3[i];
					yq = g * pY1[i] + a * pY2[i] + b * pY3[i];
					zq = g * pZ1[i] + a * pZ2[i] + b * pZ3[i];

					x_x0 = xq - pXO[j];
					y_y0 = yq - pYO[j];
					z_z0 = zq - pZO[j];

					x2 = x_x0 * x_x0;
					y2 = y_y0 * y_y0;
					z2 = z_z0 * z_z0;

					r = sqrt(x2 + y2 + z2);
					
					exp_ikr.re = cos(K_0 * r);
					exp_ikr.im = (-1)*sin(K_0 * r);

					funG = exp_ikr / (4 * pi * r);
					r2 = r * r;
					r4 = r2 * r2;

					//
					//НАЧАЛО КОСТИНЫ ФОРМУЛЫ
					//
					coeffA = (-1) * imOne * K_0 / r - 1.0 / r2;
					coeffB = (3.0 + 3.0 * imOne * K_0 * r - K2 * r2) / r4;

					dx = x_x0 * coeffA * funG;
					dy = y_y0 * coeffA * funG;
					dz = z_z0 * coeffA * funG;

					dxx = (x2 * coeffB + coeffA) * funG;
					dyy = (y2 * coeffB + coeffA) * funG;
					dzz = (z2 * coeffB + coeffA) * funG;

					dxy = (x_x0 * y_y0 * coeffB) * funG;
					dxz = (x_x0 * z_z0 * coeffB) * funG;
					dyz = (y_y0 * z_z0 * coeffB) * funG;
					//
					// КОНЕЦ
					//					
					exc = Ekoeff * (K2 * ix * funG + ix * dxx + iy * dxy + iz * dxz) * dS - (mz * dy - my * dz) * dS; //-
					eyc = Ekoeff * (K2 * iy * funG + iy * dyy + iz * dyz + ix * dxy) * dS - (mx * dz - mz * dx) * dS; //-
					ezc = Ekoeff * (K2 * iz * funG + iz * dzz + ix * dxz + iy * dyz) * dS - (my * dx - mx * dy) * dS; //-

					hxc = Mukoeff * (K2 * mx * funG + mx * dxx + my * dxy + mz * dxz) * dS + (iz * dy - iy * dz) * dS; //+
					hyc = Mukoeff * (K2 * my * funG + mx * dxy + my * dyy + mz * dyz) * dS + (ix * dz - iz * dx) * dS; //+
					hzc = Mukoeff * (K2 * mz * funG + mx * dxz + my * dyz + mz * dzz) * dS + (iy * dx - ix * dy) * dS; //+

					ex = ex + w * exc;
					ey = ey + w * eyc;
					ez = ez + w * ezc;

					hx = hx + w * hxc;
					hy = hy + w * hyc;
					hz = hz + w * hzc;

					//Точка 3
					a = 0.20000000f;        //
					b = 0.60000000f;        // координаты
					g = 0.20000000f;        //
					w = 0.52083333f;       // вес

					xq = g * pX1[i] + a * pX2[i] + b * pX3[i];
					yq = g * pY1[i] + a * pY2[i] + b * pY3[i];
					zq = g * pZ1[i] + a * pZ2[i] + b * pZ3[i];

					x_x0 = xq - pXO[j];
					y_y0 = yq - pYO[j];
					z_z0 = zq - pZO[j];

					x2 = x_x0 * x_x0;
					y2 = y_y0 * y_y0;
					z2 = z_z0 * z_z0;

					r = sqrt(x2 + y2 + z2);
					
					exp_ikr.re = cos(K_0 * r);
					exp_ikr.im = (-1)*sin(K_0 * r);

					funG = exp_ikr / (4 * pi * r);
					r2 = r * r;
					r4 = r2 * r2;

					//
					//НАЧАЛО КОСТИНЫ ФОРМУЛЫ
					//
					coeffA = (-1) * imOne * K_0 / r - 1.0 / r2;
					coeffB = (3.0 + 3.0 * imOne * K_0 * r - K2 * r2) / r4;

					dx = x_x0 * coeffA * funG;
					dy = y_y0 * coeffA * funG;
					dz = z_z0 * coeffA * funG;

					dxx = (x2 * coeffB + coeffA) * funG;
					dyy = (y2 * coeffB + coeffA) * funG;
					dzz = (z2 * coeffB + coeffA) * funG;

					dxy = (x_x0 * y_y0 * coeffB) * funG;
					dxz = (x_x0 * z_z0 * coeffB) * funG;
					dyz = (y_y0 * z_z0 * coeffB) * funG;
					//
					// КОНЕЦ
					//					
					exc = Ekoeff * (K2 * ix * funG + ix * dxx + iy * dxy + iz * dxz) * dS - (mz * dy - my * dz) * dS; //-
					eyc = Ekoeff * (K2 * iy * funG + iy * dyy + iz * dyz + ix * dxy) * dS - (mx * dz - mz * dx) * dS; //-
					ezc = Ekoeff * (K2 * iz * funG + iz * dzz + ix * dxz + iy * dyz) * dS - (my * dx - mx * dy) * dS; //-

					hxc = Mukoeff * (K2 * mx * funG + mx * dxx + my * dxy + mz * dxz) * dS + (iz * dy - iy * dz) * dS; //+
					hyc = Mukoeff * (K2 * my * funG + mx * dxy + my * dyy + mz * dyz) * dS + (ix * dz - iz * dx) * dS; //+
					hzc = Mukoeff * (K2 * mz * funG + mx * dxz + my * dyz + mz * dzz) * dS + (iy * dx - ix * dy) * dS; //+

					ex = ex + w * exc;
					ey = ey + w * eyc;
					ez = ez + w * ezc;

					hx = hx + w * hxc;
					hy = hy + w * hyc;
					hz = hz + w * hzc;


					//Точка 4
					a = 0.20000000f;        //
					b = 0.20000000f;        // координаты
					g = 0.60000000f;        //
					w = 0.52083333f;        // вес

					xq = g * pX1[i] + a * pX2[i] + b * pX3[i];
					yq = g * pY1[i] + a * pY2[i] + b * pY3[i];
					zq = g * pZ1[i] + a * pZ2[i] + b * pZ3[i];

					x_x0 = xq - pXO[j];
					y_y0 = yq - pYO[j];
					z_z0 = zq - pZO[j];

					x2 = x_x0 * x_x0;
					y2 = y_y0 * y_y0;
					z2 = z_z0 * z_z0;

					r = sqrt(x2 + y2 + z2);
										
					exp_ikr.re = cos(K_0 * r);
					exp_ikr.im = (-1)*sin(K_0 * r);

					funG = exp_ikr / (4 * pi * r);
					r2 = r * r;
					r4 = r2 * r2;

					//
					//НАЧАЛО КОСТИНЫ ФОРМУЛЫ
					//
					coeffA = (-1) * imOne * K_0 / r - 1.0 / r2;
					coeffB = (3.0 + 3.0 * imOne * K_0 * r - K2 * r2) / r4;

					dx = x_x0 * coeffA * funG;
					dy = y_y0 * coeffA * funG;
					dz = z_z0 * coeffA * funG;

					dxx = (x2 * coeffB + coeffA) * funG;
					dyy = (y2 * coeffB + coeffA) * funG;
					dzz = (z2 * coeffB + coeffA) * funG;

					dxy = (x_x0 * y_y0 * coeffB) * funG;
					dxz = (x_x0 * z_z0 * coeffB) * funG;
					dyz = (y_y0 * z_z0 * coeffB) * funG;
					//
					// КОНЕЦ
					//					
					exc = Ekoeff * (K2 * ix * funG + ix * dxx + iy * dxy + iz * dxz) * dS - (mz * dy - my * dz) * dS; //-
					eyc = Ekoeff * (K2 * iy * funG + iy * dyy + iz * dyz + ix * dxy) * dS - (mx * dz - mz * dx) * dS; //-
					ezc = Ekoeff * (K2 * iz * funG + iz * dzz + ix * dxz + iy * dyz) * dS - (my * dx - mx * dy) * dS; //-

					hxc = Mukoeff * (K2 * mx * funG + mx * dxx + my * dxy + mz * dxz) * dS + (iz * dy - iy * dz) * dS; //+
					hyc = Mukoeff * (K2 * my * funG + mx * dxy + my * dyy + mz * dyz) * dS + (ix * dz - iz * dx) * dS; //+
					hzc = Mukoeff * (K2 * mz * funG + mx * dxz + my * dyz + mz * dzz) * dS + (iy * dx - ix * dy) * dS; //+

					ex = ex + w * exc;
					ey = ey + w * eyc;
					ez = ez + w * ezc;

					hx = hx + w * hxc;
					hy = hy + w * hyc;
					hz = hz + w * hzc;
				}
				else
				{
					CComplex exp_ikr;
					exp_ikr.re = cos(K_0 * r);
					exp_ikr.im = (-1)*sin(K_0 * r);

					CComplex funG = exp_ikr / (4 * pi * r);
					double r2 = r * r;
					double r4 = r2 * r2;

					//
					//НАЧАЛО КОСТИНЫ ФОРМУЛЫ
					//
					CComplex coeffA = (-1) * imOne * K_0 / r - 1.0 / r2;
					CComplex coeffB = (3.0 + 3.0 * imOne * K_0 * r - K2 * r2) / r4;

					CComplex dx = x_x0 * coeffA * funG;
					CComplex dy = y_y0 * coeffA * funG;
					CComplex dz = z_z0 * coeffA * funG;

					CComplex dxx = (x2 * coeffB + coeffA) * funG;
					CComplex dyy = (y2 * coeffB + coeffA) * funG;
					CComplex dzz = (z2 * coeffB + coeffA) * funG;

					CComplex dxy = (x_x0 * y_y0 * coeffB) * funG;
					CComplex dxz = (x_x0 * z_z0 * coeffB) * funG;
					CComplex dyz = (y_y0 * z_z0 * coeffB) * funG;
					//
					// КОНЕЦ
					//					
					ex = ex + Ekoeff * (K2 * ix * funG + ix * dxx + iy * dxy + iz * dxz) * dS - (mz * dy - my * dz) * dS; //-
					ey = ey + Ekoeff * (K2 * iy * funG + iy * dyy + iz * dyz + ix * dxy) * dS - (mx * dz - mz * dx) * dS; //-
					ez = ez + Ekoeff * (K2 * iz * funG + iz * dzz + ix * dxz + iy * dyz) * dS - (my * dx - mx * dy) * dS; //-

					hx = hx + Mukoeff * (K2 * mx * funG + mx * dxx + my * dxy + mz * dxz) * dS + (iz * dy - iy * dz) * dS; //+
					hy = hy + Mukoeff * (K2 * my * funG + mx * dxy + my * dyy + mz * dyz) * dS + (ix * dz - iz * dx) * dS; //+
					hz = hz + Mukoeff * (K2 * mz * funG + mx * dxz + my * dyz + mz * dzz) * dS + (iy * dx - ix * dy) * dS; //+					
				}

			}
			solution[6 * j + 0] = ex;
			solution[6 * j + 1] = ey;
			solution[6 * j + 2] = ez;
			solution[6 * j + 3] = hx;
			solution[6 * j + 4] = hy;
			solution[6 * j + 5] = hz;
		}
	}
	return solution;
}



