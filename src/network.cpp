#include "network.h"
#include "random.h"
#include <iostream>
#include <tclap/CmdLine.h>

void Network::resize(const size_t& s) {
	if (s>0) {
		values.resize(s);
		RNG.normal(values, 0, 1);
	}
	else TCLAP::ArgException("Impossible, taille négative");
}

bool Network::add_link(const size_t& a, const size_t& b) {
	bool success(true);
	
	if (a==b) return !success;
	if ((size()>a) and (size()>b)) {
		if (links.find(b) != links.end()) { 
			std::multimap<size_t, size_t>::iterator it1(links.find(b));
			if ((it1->first == b) and (it1->second == a)) {
				std::cerr << "Les liens existent deja !" << std::endl;
				return !success; 
			}
		}
		
		if (links.find(a) != links.end()) { 
			std::multimap<size_t, size_t>::iterator it2(links.find(a));
			if ((it2->first == a) and (it2->second == b)) {
				std::cerr << "Les liens existent deja !" << std::endl;
				return !success;
			}
		}
			 
		links.insert(std::make_pair(a, b));
		links.insert(std::make_pair(b, a));
		return success;
	} else {
		std::cerr<< "Les size_t ne sont pas dans les limites du tableau" << std::endl;
		return !success; 
	}
}

size_t Network::random_connect(const double& mean_deg) {
	links.clear();
	
	size_t compteur(0); //Compteur du nombre total de liens créés
	size_t degre(0); 
	
	std::vector<size_t> indices;
		
	for (size_t i(0); i<size(); ++i) indices.push_back(i);

	for (size_t j(0); j<size(); ++j) {
		degre = RNG.poisson(mean_deg);
		if (degre>(size()-1)) degre = (size()-1); //On ne peut pas rajouter plus de liens qu'il n'y a de valeurs dans le tableau
												  //size-1 car on ne peut pas créer de liens vers lui même 
		RNG.RandomNumbers::shuffle(indices);
		size_t nb_liens (0);
		
		for (size_t n(0); n<indices.size() and (nb_liens<degre); ++n) { 
			if (add_link(j, indices[n])) { 
				++compteur;
				++nb_liens;
			}
		}
	}
	return compteur;
}
	

size_t Network::set_values(const std::vector<double>& vec) {
	if (size()<=vec.size()) {
		for (size_t i(0); i<size(); ++i) {
			values[i] = vec[i];
		}
		return size();
	}
	else {
		for (size_t i(0); i<vec.size(); ++i) {
			values[i] = vec[i];
		}
		return vec.size();
	}
}

size_t Network::size() const {
	return values.size();
}

size_t Network::degree(const size_t &_n) const {
	return links.count(_n);
}

double Network::value(const size_t &_n) const {
	return values[_n];
}

std::vector<double> Network::sorted_values() const {
	std::vector<double> resultat(values);
	bool tri = false;
	int taille (size()); 
	while (!tri) {
		tri =true; 
		
		for (int i(0); i<(taille-1); ++i) {
			if (resultat[i]<resultat[i+1]) {
				std::swap(resultat[i], resultat[i+1]);
				tri = false;
			}
		}
		taille--;
	}
	return resultat;
}

std::vector<size_t> Network::neighbors(const size_t& a) const {
	std::vector<size_t> neighbours;
	
	for (std::multimap<size_t, size_t>::const_iterator it_ = links.cbegin(); it_!=links.cend(); ++it_) {
		if (it_->first ==a) {
			neighbours.push_back(it_->second);
		}
	}
	return neighbours;
}
