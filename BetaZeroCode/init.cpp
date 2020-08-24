#include "bits/stdc++.h"
#include "defs.h"
// this macro generates a 64 bit random integer rand() generates 15 bit integer
#define RAND_64 (	(U64)rand() | \
					(U64)rand() << 15 | \
					(U64)rand() << 30 | \
					(U64)rand() << 45 | \
					((U64)rand() & 0xf) << 60    )
					

int Sq120ToSq64[BRD_SQ_NUM]; // array generating sq 64 from sq 120 
int Sq64ToSq120[64]; // array generating sq 120 from sq 64

U64 SetMask[64]; // array setting mask in a bit
U64 ClearMask[64]; // array clearing a bit

U64 PieceKeys[13][120]; // array for generating keys for pieces
U64 SideKey; // array for generating key for which side is going on
U64 CastleKeys[16]; // array for generating key for castlings 

int FilesBrd[BRD_SQ_NUM]; // tells the file of the square
int RanksBrd[BRD_SQ_NUM]; // tells the rank of the square

U64 FileBBMask[8];
U64 RankBBMask[8];

U64 BlackPassedMask[64];
U64 WhitePassedMask[64];
U64 IsolatedMask[64];

void InitEvalMasks() {

	int sq, tsq, r, f;
	
	for(sq = 0; sq < 8; ++sq) {		
        FileBBMask[sq] = 0ULL; 
		RankBBMask[sq] = 0ULL; 
	}
	
	for(r = RANK_8; r >= RANK_1; r--) {
        for (f = FILE_A; f <= FILE_H; f++) {
            sq = r * 8 + f;
            FileBBMask[f] |= (1ULL << sq);
            RankBBMask[r] |= (1ULL << sq);
        }
	}
	
	for(sq = 0; sq < 64; ++sq) {
		IsolatedMask[sq] = 0ULL; 
		WhitePassedMask[sq] = 0ULL; 
		BlackPassedMask[sq] = 0ULL;
    }

	for(sq = 0; sq < 64; ++sq) {
		tsq = sq + 8;
		
        while(tsq < 64) {
            WhitePassedMask[sq] |= (1ULL << tsq);
            tsq += 8;
        }

        tsq = sq - 8;
        while(tsq >= 0) {
            BlackPassedMask[sq] |= (1ULL << tsq);
            tsq -= 8;
        }

        if(FilesBrd[SQ120(sq)] > FILE_A) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[SQ120(sq)] - 1];

            tsq = sq + 7;
            while(tsq < 64) {
                WhitePassedMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 9;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
		
        if(FilesBrd[SQ120(sq)] < FILE_H) {
            IsolatedMask[sq] |= FileBBMask[FilesBrd[SQ120(sq)] + 1];

            tsq = sq + 9;
            while(tsq < 64) {
                WhitePassedMask[sq] |= (1ULL << tsq);
                tsq += 8;
            }

            tsq = sq - 7;
            while(tsq >= 0) {
                BlackPassedMask[sq] |= (1ULL << tsq);
                tsq -= 8;
            }
        }
	}
	
}


void InitFilesRanksBrd() {
	
	int index = 0;
	int file = FILE_A;
	int rank = RANK_1;
	int sq = A1;
	int sq64 = 0;
	// setting everything to offboard first	
	for(index = 0; index < BRD_SQ_NUM; ++index) {
		FilesBrd[index] = OFFBOARD;
		RanksBrd[index] = OFFBOARD;
	}
	// simple enough function to get rank and file of a square
	for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FR2SQ(file,rank);
			FilesBrd[sq] = file;
			RanksBrd[sq] = rank;
		}
	}
}


void InitHashKeys() {
	// initializing hashes with random numbers
	int index = 0;
	int index2 = 0;
	for(index = 0; index < 13; ++index) {
		for(index2 = 0; index2 < 120; ++index2) {
			PieceKeys[index][index2] = RAND_64;
		}
	}
	SideKey = RAND_64;
	for(index = 0; index < 16; ++index) {
		CastleKeys[index] = RAND_64;
	}

}
void InitBitMasks() {
    // initializing bitmasks
	int index = 0;
	
	for(index = 0; index < 64; index++) {
		SetMask[index] = 0ULL;
		ClearMask[index] = 0ULL;
	}
	
	for(index = 0; index < 64; index++) {
		SetMask[index] |= (1ULL << index);
		ClearMask[index] = ~SetMask[index];
	}
}


void InitSq120to64(){
    //initialization
    int index=0;
    int file= FILE_A;
    int rank= RANK_1;
    int sq=A1;
    int sq64=0;

    //failsafes
    for(index=0; index<BRD_SQ_NUM; ++index) Sq120ToSq64[index]=65;
    for(index=0;index<64;++index) Sq64ToSq120[index]=120;
    

    //initializing the global array Sq120toSq64 and Sq64toSq120
    for(rank = RANK_1; rank <= RANK_8; ++rank) {
		for(file = FILE_A; file <= FILE_H; ++file) {
			sq = FR2SQ(file,rank);
			Sq64ToSq120[sq64] = sq;
			Sq120ToSq64[sq] = sq64;
			sq64++;
		}
	}


}

void AllInit(){
    InitSq120to64();
    InitBitMasks();
    InitHashKeys();
	InitFilesRanksBrd();
	InitEvalMasks();
	InitMvvLva();
}