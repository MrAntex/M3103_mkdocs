#include <unordered_map>
#include <iostream>
#include <fstream>
#include <cstring>
#include <chrono>
#include <getopt.h>
#include <vector>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>

int main(int argc, char * argv[]){

	uint maxfind = 1000;
	uint maxinsert = 1000;
	uint maxfindall = 300;
	uint maxinsertall = 300;
	int options;
	bool mode = 0;
	bool datamode = 0;
	int pas = 1;
	int mininsert = 10;
	int minfind = 10;
	uint iterations = 10;
	std::vector<std::string> livre;
	std::string writefic = "occurences.txt";
	std::string motcherche = "";
	std::string ficdatainsert = "datainsert.dat";
	std::string ficdatafind = "datafind.dat";
	std::string ficdataload = "dataload.dat";
	std::string ficdatall = "data.dat";

	while((options = getopt(argc, argv, "i:a:z:f:cm:dp:")) != -1){
		switch(options){
			case 'i':
				iterations = std::stoi(optarg);
				break;
			case 'a':
				mininsert = std::stoi(optarg);
				break;
			case 'z':
				maxinsertall = std::stoi(optarg);
				break;
			case 'f':
				writefic = optarg;
				break;
			case 'c':
				mode = true;
				break;
			case 'm':
				motcherche = optarg;
				break;
			case 'd':
				datamode = true;
				break;
			case 'p':
				pas = std::stoi(optarg);
				break;



		}
	}
minfind = mininsert;
maxfindall = maxinsertall;

	std::ifstream ifs;
	std::ofstream ofs;
	std::ofstream ficinsert, ficfind, ficload, ficall;

	std::cout << "Réinitialisation du fichier d'occurences..." << std::endl;
	if( remove(writefic.c_str()) != 0 )
    perror( "Echec de l'opération :/ " );
  else
    puts( "Fichier réinitialisé avec succès !" );

	ofs.open(writefic, std::ofstream::out);

	if(datamode){
		std::cout << "Réinitialisation du fichier de temps d'insertion..." << std::endl;
		if( remove(ficdatainsert.c_str()) != 0 )
	    perror( "Echec de l'opération :/ " );
	  else
	    puts( "Fichier réinitialisé avec succès !" );

		std::cout << "Réinitialisation du fichier de temps de recherche..." << std::endl;
		if( remove(ficdatafind.c_str()) != 0 )
	    perror( "Echec de l'opération :/ " );
		else
	    puts( "Fichier réinitialisé avec succès !" );

		std::cout << "Réinitialisation du fichier de taux de charge..." << std::endl;
		if( remove(ficdataload.c_str()) != 0 )
			perror( "Echec de l'opération :/ " );
		else
			puts( "Fichier réinitialisé avec succès !" );

		std::cout << "Réinitialisation du fichier global de données..." << std::endl;
		if( remove(ficdatall.c_str()) != 0 )
			perror( "Echec de l'opération :/ " );
		else
			puts( "Fichier réinitialisé avec succès !" );

		ficinsert.open(ficdatainsert, std::ofstream::out);
		ficfind.open(ficdatafind, std::ofstream::out);
		ficload.open(ficdataload, std::ofstream::out);
		ficall.open(ficdatall, std::ofstream::out);

		ficall << "#Données de temps d'insertion, de recherche et de taux de charge sur une table de hachage."<< std::endl;
		ficall << "#Taille_map	Insertion	Recherche	Charge" << std::endl;
	}


	ifs.open("Jules_Verne_Voyage_au_centre_de_la_Terre.txt", std::ifstream::in);

	if(!ifs.is_open()){return 1;}

	uint nbfind = 0;
	uint nbinsert = 0;

	std::chrono::nanoseconds time_find;
	std::chrono::nanoseconds time_insert;

	std::string mot;
	std::unordered_map<std::string, int> map;

	if(!mode){

	while(ifs >> mot && ifs.good()){
		//ifs >> mot;
		livre.push_back(mot);
		std::unordered_map<std::string, int>::const_iterator found;

		std::chrono::steady_clock::time_point debfind = std::chrono::steady_clock::now();
		found = map.find(mot);
		std::chrono::steady_clock::time_point endfind = std::chrono::steady_clock::now();

		if(nbfind <= maxfind){
		time_find = time_find + std::chrono::duration_cast<std::chrono::nanoseconds>(endfind - debfind);
		nbfind++;
	}

		if(found == map.end()){
			std::chrono::steady_clock::time_point debinsert = std::chrono::steady_clock::now();
			map[mot] = 1;
			std::chrono::steady_clock::time_point endinsert = std::chrono::steady_clock::now();

			if(nbinsert <= maxinsert){
				time_insert = time_insert + std::chrono::duration_cast<std::chrono::nanoseconds>(endinsert - debinsert);
				nbinsert++;
			}
	}
		else
		map[mot]++;
	}

	for (auto& x: map)
    ofs << x.first << ": " << x.second << std::endl;
if(motcherche != ""){
		std::unordered_map<std::string, int>::const_iterator motch = map.find(motcherche);
		std::cout<<"Nombre de mots différents : "<<map.size()<<std::endl;
		if(motch != map.end())
		std::cout << "Nombre d'occurences du mot '"<<motcherche<<"' : "<<map[mot]<<std::endl;
		else
		std::cout << "Le mot : '" << motcherche <<"' ne figure pas dans le fichier." << std::endl;
	}

	}
	else{
		while(ifs >> mot && ifs.good())
		livre.push_back(mot);

std::vector<std::chrono::nanoseconds> tabtempsinsert;
std::vector<std::chrono::nanoseconds> tabtempsfind;

std::cout << "Insertion..." << std::endl;


		for(uint w=mininsert;w<=maxinsertall;w=w+pas){

				tabtempsfind.clear();
				tabtempsinsert.clear();

				if(datamode){
					maxinsert = w;
				}
				else{w = maxinsertall;}
					time_insert = std::chrono::nanoseconds::zero();

		std::unordered_map<std::string, int>::const_iterator found;

		tabtempsfind.clear();
		for(uint i=1;i<=iterations;++i){

			int chosen = (std::rand() % livre.size());
			mot = livre[chosen];

			std::chrono::steady_clock::time_point debfind = std::chrono::steady_clock::now();
			found = map.find(mot);
			std::chrono::steady_clock::time_point endfind = std::chrono::steady_clock::now();
			std::chrono::nanoseconds time_find = std::chrono::duration_cast<std::chrono::nanoseconds>(endfind - debfind);

			tabtempsfind.push_back(time_find);
}
			time_find = std::chrono::nanoseconds::zero();
			for(auto& n : tabtempsfind)
				time_find += n;

			double totimefind = time_find.count() / (double)tabtempsfind.size();

			tabtempsinsert.clear();
		for(uint i=1;i<=iterations;++i){

			int chosen = (std::rand() % livre.size());
			mot = livre[chosen];

				std::unordered_map<std::string, int> newmap(map);
				std::chrono::steady_clock::time_point debinsert = std::chrono::steady_clock::now();
					newmap[mot]++;
				std::chrono::steady_clock::time_point endinsert = std::chrono::steady_clock::now();
				std::chrono::nanoseconds time_insert = std::chrono::duration_cast<std::chrono::nanoseconds>(endinsert - debinsert);
				newmap.clear();
				tabtempsinsert.push_back(time_insert);
		}
				time_insert = std::chrono::nanoseconds::zero();
				for(auto& n : tabtempsinsert)
					time_insert += n;

				double totimeinsert = time_insert.count() / (double)tabtempsinsert.size();

			float progress = (float)(w-mininsert)/(maxinsertall-mininsert);
			int pos = 60 * progress;

			std::cout << "[";
			for (int k = 0; k < 60; ++k) {
        if (k < pos) std::cout << "#";
        else if (k == pos) std::cout << ">";
        else std::cout << " ";
    }
    std::cout << "] " << std::ceil(progress * 100.0) << " % | "<<std::ceil((w-mininsert)/pas)<<"/"<<std::ceil((maxinsertall-mininsert)/pas)<<"\r";
    std::cout.flush();

		if(datamode){
		ficinsert << map.size()-1 << " " << time_insert.count()/100000000 <<std::endl;
		ficfind << map.size()-1 << " " << (double)time_find.count()/100000000 << std::endl;
		ficload << map.size()-1 << " " << map.load_factor() << std::endl;
		ficall<<map.size()-1<<" "<<totimeinsert/100000000<<" "<<totimefind/100000000<<" "<<map.load_factor()<<std::endl;
		}

		}


	std::cout <<std::endl<< "Insertions et recherches terminées." << std::endl;

	std::cout << std::endl << map.size() << " mots différents" << std::endl;
	std::cout << "Temps d'insertion pour "<<maxinsert<<" mots : "<< time_insert.count() << "ns ("<<(double)time_insert.count()/100000000<<"s)" << std::endl;
	std::cout << "Temps de recherche pour "<<maxfind<<" mots : " << time_find.count() << "ns ("<<(double)time_find.count()/100000000<<"s)" << std::endl;

}
}
