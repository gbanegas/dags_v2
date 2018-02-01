#include "util.h"
#include "gf.h"

//weight computes the weight of a sequence of elements of type unsigned char
int weight(unsigned char *r, int size)
{
	int i = 0, w = 0;
	for (i = 0; i < size; i++)
	{
		if (r[i] != 0)
			w++;
	}
	return w;
}

//random_m generate randomly a sequence of size element of F_q
unsigned char *random_m(int size)
{
	unsigned char *r = (unsigned char *)malloc(size);
	int i;
	randombytes(r, size);
	for (i = 0; i < size; i++)
	{
		r[i] = r[i] & gf_ord_sf;
	}
	return r;
}

//indice_in_vec test if element is in tab
int indice_in_vec(unsigned int *v, int j, int size)
{
	int i;
	for (i = 0; i < size; i++)
	{
		if (v[i] == j)
			return 1;
	}
	return 0;
}

//random_e
unsigned char *random_e(int size, int q, int w, unsigned char *sigma)
{
	unsigned char *e = (unsigned char *)calloc(size, sizeof(unsigned char));
	unsigned int *v = (unsigned int *)calloc(size, sizeof(unsigned int));
	int i, j = 0, k = 0, jeton = 0;

	for (i = 0; i < size; i++)
	{
		if (sigma[i] % q == 0)
		{
			continue;
		}
		if (j == w)
		{
			break;
		}
		do
		{
			jeton = (sigma[k + 1] ^ (sigma[k] << 4)) % size;
			k++;
		} while (indice_in_vec(v, jeton, j + 1) == 1); //Only check j elements
		v[j] = jeton;
		e[jeton] = sigma[i] % q;
		jeton = 0;
		j++;
	}
	free(v);
	return e;
}

//TODO Gustavo can this be simpler like the function for store_sk
void store_pk(binmat_t M, unsigned char *pk)
{
	int i, j, k, p, d, a = 0;
	k = code_dimension / (order);
	p = (code_length - code_dimension) / 4;
	gf c1 = 0, c2 = 0, c3 = 0, c4 = 0;
	unsigned char c = 0;

	gf *L;
	L = (gf *)calloc((code_length - code_dimension), sizeof(gf));
	for (i = 0; i < k; i++)
	{
		d = i * (order);
		for (j = 0; j < p; j++)
		{
			c1 = M.coeff[4 * j][d];
			L[4 * j] = c1;
			c2 = M.coeff[4 * j + 1][d];
			L[4 * j + 1] = c2;
			c3 = M.coeff[4 * j + 2][d];
			L[4 * j + 2] = c3;
			c4 = M.coeff[4 * j + 3][d];
			L[4 * j + 3] = c4;
			c = (c1 << 2) ^ (c2 >> 4);
			//printf("--c= %d \t",c);
			pk[a] = c;
			a += 1;
			c1 = (c2 & 15);
			c = (c1 << 4) ^ (c3 >> 2);
			//printf("--c= %d \t",c);
			pk[a] = c;
			a += 1;
			c1 = (c3 & 3);
			c = (c1 << 6) ^ c4;
			//printf("--c= %d \t",c);
			pk[a] = c;

			a += 1;
		}
		//affiche_vecteur(L,code_length-code_dimension);
		//printf(" \n");
	}
	free(L);
}

void recup_pk(const unsigned char *pk, binmat_t G)
{
	int a = 0;
	int i, j, k, p, l, m, q;
	binmat_t M;
	M = matrix_init(code_dimension, code_length - code_dimension);
	k = code_dimension / (order);
	p = (code_length - code_dimension) / 4;
	gf c1 = 0, c2 = 0, c3 = 0, c4 = 0, tmp1 = 0, tmp2 = 0;
	q = (code_length - code_dimension) / (order);
	unsigned char c = 0;
	gf *sig, *Sig_all_line;
	sig = (gf *)calloc((order), sizeof(gf));
	Sig_all_line = (gf *)calloc((code_length - code_dimension), sizeof(gf));
	for (i = 0; i < k; i++)
	{
		for (j = 0; j < p; j++)
		{
			c = pk[a];
			//printf("--c= %d \t",c);
			c1 = c >> 2;
			Sig_all_line[4 * j] = c1;
			tmp1 = (c & 3);
			a += 1;
			c = pk[a];
			//printf("--c= %d \t",c);
			c2 = (tmp1 << 4) ^ (c >> 4);
			Sig_all_line[4 * j + 1] = c2;
			tmp2 = c & 15;
			a += 1;
			c = pk[a];
			a += 1;
			//printf("--c= %d \t",c);
			c3 = (tmp2 << 2) ^ (c >> 6);
			Sig_all_line[4 * j + 2] = c3;
			c4 = c & 63;
			Sig_all_line[4 * j + 3] = c4;
		}
		//affiche_vecteur(Sig_all_line,code_length-code_dimension);
		//printf(" \n");
		for (l = 0; l < q; l++)
		{
			for (m = 0; m < (order); m++)
			{
				sig[m] = Sig_all_line[l * (order) + m];
			}
			//affiche_vecteur(sig,order);
			quasi_dyadic_bloc_mat(order, M, sig, l * (order), i * (order));
		}
	}
	for (i = 0; i < G.rown; i++)
	{

		G.coeff[i][i] = 1;
		for (j = M.rown; j < G.coln; j++)
		{
			G.coeff[i][j] = M.coeff[i][j - M.rown];
		}
	}
	free(Sig_all_line);
	mat_free(M);
	free(sig);
}

void store_sk(binmat_t H_alt, unsigned char *sk)
{
	unsigned int sk_loc = 0, i;
	memcpy(sk, &H_alt.rown, sizeof(H_alt.rown));
	sk_loc += sizeof(H_alt.rown);
	memcpy(sk + sk_loc, &H_alt.coln, sizeof(H_alt.coln));
	sk_loc += sizeof(H_alt.coln);
	for (i = 0; i < H_alt.rown; i++)
	{
		memcpy(sk + sk_loc, H_alt.coeff[i], H_alt.coln * sizeof(gf));
		sk_loc += H_alt.coln * sizeof(gf);
	}
}

binmat_t read_sk(const unsigned char *sk)
{
	unsigned int rown = ((unsigned int *)sk)[0];
	unsigned int coln = ((unsigned int *)sk)[1];
	unsigned int sk_loc = 2 * sizeof(unsigned int);
	unsigned int i;
	binmat_t H_alt = matrix_init(rown, coln);
	for (i = 0; i < H_alt.rown; i++)
	{
		memcpy(H_alt.coeff[i], sk + sk_loc, H_alt.coln * sizeof(gf));
		sk_loc += H_alt.coln * sizeof(gf);
	}
	return H_alt;
}
void set_y_from_uvz(gf * u, gf* v, gf * Z, gf* y){

        gf* z = (gf *)calloc(code_length, sizeof(gf));
        gf pol,aux;
	int i,j;
	//int st = order * pol_deg;

	for (i = 0; i < n0_val; i++)
	    {
		for (j = 0; j < order; j++)
		{
		    z[i * (order) + j] = Z[i];
		}
	    }
	
        for (i = 0; i <  code_length; i++)
        {
            pol =1;
            for (j = 0; j < order; j++)
            {
		aux = v[i]^u[j];
                pol = gf_mult(pol,aux);
            }
	y[i] = gf_mult(z[i], gf_pow(gf_inv(pol),pol_deg));

        }
}

void set_sk_from_vy(gf * v, gf * y,unsigned char * sk){
	int i, a=0;
	gf c1, c2;
        //int order1 =(order);
	unsigned char c;


	for(i=0;i<(code_length/2);i++){
                        c1=v[2*i];
			c2=v[2*i+1];
			c=c1>>4;
			sk[a]=c;
			a+=1;
			c1=c1&15;
			c=(c1<<4)^(c2>>8);
			sk[a]=c;
			a+=1;
			c=c2&255;
			sk[a]=c;
			a+=1;
			}
	for(i=0;i<(code_length/2);i++){
                        c1=y[2*i];
			c2=y[2*i+1];
			c=c1>>4;
			sk[a]=c;
			a+=1;
			c1=c1&15;
			c=(c1<<4)^(c2>>8);
			sk[a]=c;
			a+=1;
			c=c2&255;
			sk[a]=c;
			a+=1;
			}

	
}



void set_vy_from_sk(gf* v, gf * y, const unsigned char * sk){
	int i,a=0;
	unsigned char c;
	gf c1, c2, c3;
	for(i=0;i<(code_length/2);i++){
            c=sk[a];
            c1=c;
			a+=1;
			c=sk[a];
            c2=c;
			a+=1;
			c=sk[a];
            c3=c;
			a+=1;
			v[2*i]=(c1<<4)^(c2>>4);
			c1=c2&15;
			v[2*i+1]=(c1<<8)^c3;
			}
        for(i=0;i<(code_length/2);i++){
            c=sk[a];
            c1=c;
			a+=1;
			c=sk[a];
            c2=c;
			a+=1;
			c=sk[a];
            c3=c;
			a+=1;
			y[2*i]=(c1<<4)^(c2>>4);
			c1=c2&15;
			y[2*i+1]=(c1<<8)^c3;
			}
	
	
	
}



gf sum_vect_element(gf* w,int length){
   gf tmp=0;
   int j=0;
        for (j = 0; j < length; j++)
        {
            tmp ^= w[j];
        }
   return tmp;
}
