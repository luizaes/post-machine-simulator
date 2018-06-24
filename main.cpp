#include <iostream>
#include <list>
#include <queue>
#include <vector>
#include <cstring>

using namespace std;

/* Definições */
#define NONE '$'
#define ACCEPT 1
#define REJECT 0

typedef struct state {
	string nome;
	char simboloLeitura;
	char simboloAddFila;
} state;

typedef struct node {
	string nome;
	list<state> fluxos;
} node;

vector<node> diagramaFluxo;
queue<char> filaX;
string partida, aceitacao;

int percorreDiagrama(int num);

int main() {

	string estado, entrada, transicao, erroText;
	int i, qtdEstados, qtdTransicoes, j, resultado;
	char *stringDividida, aux;
	node nodeAux;
	state stateAux;
	bool erro = false, acheiEstado = false;
	char simbolo, simboloFila = NONE, auxChar;

	/* Leio a quantidade de estados que a máquina possui */
	cin >> qtdEstados;

	/* Leio o nome de cada um dos estados, guardando na lista de adjacência */
	for (i = 0; i < qtdEstados; i++) {
		cin >> estado;
		nodeAux.nome = estado;
		diagramaFluxo.push_back(nodeAux);
	}

	/* Guardo o estado inicial */
	cin >> partida;

	/* Guardo o estado de aceitação - neste caso não temos um estado de rejeição, 
	   a máquina morre por indefinição para rejeitar uma palavra */
	cin >> aceitacao;

	/* Guardo quantas "transições" ou ligações existem no diagrama */
	cin >> qtdTransicoes;

	/* Descrevo cada ligação */
	for(i = 0; i < qtdTransicoes; i++) {
		cin >> transicao;
		for (j = 0; j < qtdEstados; j++) {

			/* Se o estado especificado na transição existe */
			if(diagramaFluxo[j].nome.compare(transicao) == 0) {
				acheiEstado = true;

				/* Caso a transição comece com um estado do tipo READ */
				if(transicao.front() == 'R' || transicao.front() == 'r') {
					cin >> simbolo;
					getline(cin, transicao);
					if(transicao.compare(partida) == 0) {
						erro = true;
						erroText = "[ERRO] Não é possível ter uma transição que vá para o estado de partida!";
						break;
					} else if(transicao.compare(" " + aceitacao) != 0 && (transicao.at(1) == 'A' || transicao.at(1) == 'a')) {
						simboloFila = transicao.substr(transicao.length()-1, 1).front();
						transicao = transicao.substr(1, transicao.length()-3);
					} else {
						transicao = transicao.substr(1, transicao.length()-1);
					}
					stateAux.nome = transicao;
					stateAux.simboloLeitura = simbolo;
					stateAux.simboloAddFila = simboloFila;
					simboloFila = NONE;
					diagramaFluxo[j].fluxos.push_back(stateAux);

				/* Caso a transição comece com um estado do tipo START (partida) */
				} else if(transicao.compare(partida) == 0) {
					getline(cin, transicao);
					if(transicao.at(1) == 'A' || transicao.at(1) == 'a') {
						simboloFila = transicao.substr(transicao.length()-1, 1).front();
						transicao = transicao.substr(1, transicao.length()-3);
					}
					stateAux.nome = transicao;
					stateAux.simboloLeitura = NONE;
					stateAux.simboloAddFila = simboloFila;
					simboloFila = NONE;
					diagramaFluxo[j].fluxos.push_back(stateAux);
				}  else if(transicao.compare(aceitacao) == 0) {
					erro = true;
					erroText = "[ERRO] O estado de aceitação não pode ter nenhuma transição para outro estado!";
					break;

				/* Caso a transição comece com um estado do tipo ADD */
				} else if(transicao.front() == 'A' || transicao.front() == 'a') {
					cin >> simbolo;
					getline(cin, transicao);
					if(transicao.compare(" " + partida) == 0) {
						erro = true;
						erroText = "[ERRO] Não é possível ter uma transição que vá para o estado de partida!";
						break;
					} else if(transicao.at(1) == 'A' || transicao.at(1) == 'a') {
						simboloFila = transicao.substr(transicao.length()-1, 1).front();
						transicao = transicao.substr(1, transicao.length()-3);
					} else {
						transicao = transicao.substr(1, transicao.length()-1);
					}
					stateAux.nome = transicao;
					stateAux.simboloLeitura = NONE;
					stateAux.simboloAddFila = simboloFila;
					simboloFila = NONE;
					diagramaFluxo[j].fluxos.push_back(stateAux);
				}
				break;
			}
		}

		if(!acheiEstado) {
			erro = true;
			erroText = "[ERRO] O estado da transição especificada não existe!";
			break;
		} else if(erro) {
			break;
		}
	}

	/* No caso de algum erro */ 
	if(erro) {
		cout << erro << endl;
	} else {

		/* Se nenhum problema ocorreu, verificamos se a entrada é aceita pela MP */
		cout << "[SUCESSO] Diagrama de Fluxo da Máquina de Post criado com sucesso." << endl;

		/* Pegamos a palavra de entrada */
		cin >> entrada;

		for(i = 0; entrada[i] != '\0'; i++) {
			filaX.push(entrada[i]);
		}

		for(i = 0; i < diagramaFluxo.size(); i++) {
			if(diagramaFluxo[i].nome.compare(partida) == 0) {
				break;
			}
		}

		/* Simulo a palavra através do diagrama e da fila */
		resultado = percorreDiagrama(i);
	}

	if(resultado == ACCEPT) {
		cout << "[ACCEPT] Entrada aceita!" << endl;
	} else {
		cout << "[REJECT] Entrada rejeitada!" << endl;
	}

	return 0;
}

int percorreDiagrama(int num) {

	int i, j;
	char simbolo, simboloFila = NONE;
	bool acheiTransicao = false;
	string nome;
	list<state>::iterator it;

	/* Caso a fila esteja vazia e ainda não atingimos o estado de aceitação - rejeite
	   Caso alcançarmos o estado final - aceite */ 
	if(diagramaFluxo[num].nome.compare(aceitacao) == 0) {
		return ACCEPT;
	} else if(filaX.empty()) {
		return REJECT;
	}

	/* Caso esteja em um estado do tipo READ */
	if(diagramaFluxo[num].nome.front() == 'R' || diagramaFluxo[num].nome.front() == 'r') {
		
		/* Verifico qual o primeiro elemento da fila */
		simbolo = filaX.front();
		for(it = diagramaFluxo[num].fluxos.begin(); it != diagramaFluxo[num].fluxos.end(); it++) {
			if((*it).simboloLeitura == simbolo) {
				acheiTransicao = true;
				nome = (*it).nome;
				simboloFila = (*it).simboloAddFila;
				break;
			}
		}

		/* Se a transição não for encontrada, a MP morre por indefinição */
		if(!acheiTransicao) {
			return REJECT;
		} else {

			/* Retiro o elemento da fila, pois já foi lido */
			filaX.pop();
			for(i = 0; i < diagramaFluxo.size(); i++) {
				if(nome.compare(diagramaFluxo[i].nome) == 0) {
					if(simboloFila != NONE) {
						filaX.push(simboloFila);
					}
					return percorreDiagrama(i);
				}
			}
		}

	/* Caso esteja em um estado do tipo ADD ou START */
	} else {
		nome = diagramaFluxo[num].fluxos.front().nome;
		simboloFila = diagramaFluxo[num].fluxos.front().simboloAddFila;
		for(i = 0; i < diagramaFluxo.size(); i++) {
			if(nome.compare(diagramaFluxo[i].nome) == 0) {
				if(simboloFila != NONE) {
					filaX.push(simboloFila);
				}
				return percorreDiagrama(i);
			}
		}
	}

} 