#include<iostream>
#include<fstream>
#include<vector>
#include<algorithm>

//#define GOTHAM

const int MAX = 4;
const int NIL = -1;

using namespace std;

typedef char byte;
typedef int num;

struct bpnode{
	vector<num> ks, pos; /* */
	num p, selfpos;
	
	bpnode(): ks(MAX), pos(MAX){
		pos.assign(MAX, -1);
	}
	
	void leia(fstream &F){
		F.seekg(selfpos);
		
		int i;
		num nsize = (num) sizeof(num);
		long bufsize = nsize * ((MAX + 1) << 1);
		byte *buffer = new byte[bufsize];
		
		F.read(buffer, bufsize);
		
		/* Read ks.size() */
		num KN = 0;
		for(i = 0; i < nsize; i++){
			KN <<= 8;
			KN += buffer[i];
		}
		ks.reserve(KN);
		
		/* Write elements in ks */
		for(i = 0; i < KN; i++){
			ks[i] = 0;
			for(int j = 0; j < nsize; j++){
				ks[i] <<= 8;
				ks[i] += buffer[nsize + j];
			}
		}
		
		/* Write elements in pos */
		for(i = 0; i < MAX; i++){
			pos[i] = 0;
			for(int j = 0; j < nsize; j++){
				pos[i] <<= 8;
				pos[i] += buffer[nsize + (nsize * MAX) + j];
			}
		}
		
		p = 0;
		for(i = 0; i < nsize; i++){
			p <<= 8;
			p += buffer[nsize + (2 * nsize * MAX) + i];
		}
		
		delete[] buffer;
	}
	
	void escreva(fstream &F){
		F.seekg(selfpos);
		
		int i;
		num nsize = (num) sizeof(num);
		long bufsize = nsize * ((MAX + 1) << 1);
		byte *buffer = new byte[bufsize];
		
		/* Write ks.size() */
		num KN = ks.size();
		for(i = 0; i < nsize; i++){
			buffer[i] = (KN >> (8 * ((nsize-1)-i))) & 0xFF;
		}
		
		/* Write elements in ks */
		for(i = 0; i < MAX; i++){
			for(int j = 0; j < nsize; j++){
				buffer[nsize + j] = (ks[i] >> (8 * ((nsize-1)-j))) & 0xFF;
			}
		}
		
		/* Write elements in pos */
		for(i = 0; i < MAX; i++){
			for(int j = 0; j < nsize; j++){
				buffer[nsize + (nsize * MAX) + j] = 
					(pos[i] >> (8 * ((nsize-1)-j))) & 0xFF;
			}
		}
		
		for(i = 0; i < nsize; i++)
			buffer[nsize + (2 * nsize * MAX) + i] = 
				(p >> (8 * ((nsize-1)-i))) & 0xFF;
		
		F.write(buffer, bufsize);
		delete[] buffer;
	}
	
	void aloca(fstream &F){
		F.seekg(0, F.end);
		selfpos = F.tellg();
	}
};

struct bptree{
	
#ifdef GOTHAM
	bpnode* search_node(bpnode* T, int val) {
		if (T->isleaf == false) {
			for(int i = 0; i < MAX && T->pos[i]; i++) {
				if (val < T->ks[i]) {
					//left of the satellite
					search_node (T->pos[i], val);
				}else if(T->size == i + 1){ 
					search_node (T->pos[i + 1], val);
				}
			}
		}
		//searched!
		return T;
	}

	int mid_node();
	void update_pointers(bpnode*, bpnode*, int);
	bpnode* split_node(bpnode*);
	bpnode* best_node_for_this_key(bpnode*, bpnode*, int);
	int insert_key2(bpnode*, int);
	void insert_key(bpnode*, int);

#endif

};



int main(int argc, char **argv){
	fstream arq ("binfile", fstream::binary | fstream::in | fstream::out);
	bpnode dummy;
	dummy.aloca(arq);
	
	for(int i = 0; i < 3; i++)
		dummy.ks[i] = (2 << 30) - i; 
	dummy.escreva(arq);
	
	bpnode dummy2;
	dummy2.selfpos = dummy.selfpos;
	
	dummy2.leia(arq);
	for(auto d: dummy.ks){
		cout << d << endl;
	}
	
	arq.close();
	return 0;
}
