/*
 *
 *
 *
 */

/* REQUISITOS DO PROGRAMA
 */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <time.h>

/* DEFINIÇÕES DO PROGRAMA
 */

// Definimos onde serão guardadas todas os dados do sistema
#define MENU "menu.txt"
#define ERRO "erro.txt"
#define TELA_INICIAL "tela_inicial.txt"
#define AJUDA "ajuda.txt"
#define CLIENTE_MENU "cliente_menu.txt"
#define CLIENTE_AJUDA "cliente_ajuda.txt"
#define ANIMAIS_TIPOS "animais_tipos.txt"
#define CONSULTA_MOTIVOS "consultas_motivos.txt"
#define CLIENTES "clientes.dat"
#define ANIMAIS "animais.dat"
#define VETERINARIOS "veterinarios.dat"
#define CONSULTAS "consultas.dat"

// Definimos um número máximo de caracteres a ser lido da entrada padrão (stdin)
#define BUFFER_MAX 256


/* CONVENÇÕES UTILIZADAS
 * 1. O CPF identifica unicamente o cliente.
 * 2. Os veterinários trabalham todos os dias úteis da semana, e no mesmo periodo.
 * 3. Não existe preferência por algum veterinário por parte de qualquer cliente e todos eles são igualmente experientes.
 * 4. O usuário do programa é quem define o horário de cada consulta.
 */

/* Erros:
 * 1. Alocação de memória: -1
 * 2. Arquivos: -2
 *
 */

/* Funções que faltam implementar
 * cadastra_veterinario()
 * 
 * 			marcar_consulta()
 * */

/* TIPOS DO PROGRAMA
 */

// Tdata
typedef struct
{
	int dia, mes, ano;
} Tdata;

// Tdata_horario
typedef struct
{
	int dia, mes, ano, horas, minutos;
} Tdata_horario;

// Tpessoa
typedef struct
{
	unsigned int id;
	char cpf[12];
	char nome[BUFFER_MAX];
	char telefone[12];
	Tdata data_de_nascimento;
} Tpessoa;

// Tcliente
typedef struct
{
	Tpessoa dados;
} Tcliente;

// Tanimal
/* Tanimal.tipo
 * 'a' - ave
 * 'g' - gato
 * 'c' - cão
 * 't' - tartaruga
 * 'j' - jabuti
 * 'h' - hamster
 * 's' - silvestre
*/
typedef struct
{
	unsigned int id, cliente_id;
	char tipo;
	char nome[BUFFER_MAX];
	Tdata_horario ultima_consulta;
} Tanimal;

// Tveterinario
typedef struct
{
	Tpessoa dados;
	float salario, valor_consulta;
} Tveterinario;

// Tconsulta
/* Tconsulta.motivo
 * 'sos' - emergência
 * 'e' - endocrinologia
 * 'ort' - ortopedia
 * 'd' - dermatologia
 * 'c' - cardiologia
 * 'odo' - odontologia
 * 'oft' - oftalmologia
 * 'onc' - oncologia
*/
typedef struct
{
	unsigned int animal_id, vet_id, cliente_id;
	char motivo[3];
	float valor;
	Tdata_horario horario;
} Tconsulta;

/* PROTÓTIPOS DE FUNÇÕES UTILIZADAS
 */

// --> FUNÇÕES DE UTILIDADE 

// printa_arquivo deve abrir um arquivo de texto e printar o seu conteúdo.
void printa_arquivo(const char * nome_de_arquivo);

// Limpa tela de comando (obs: comportamento distinto do Windows para ambientes Linux/Mac devido a falta de portabilidade)
void limpa_tela();

// Mostra uma mensagem de erro
void erro(const char * msg);

// Zera o buffer por completo
void reseta_buffer();

// Lê completamente uma linha da entrada padrão (stdin)
int leitura_linha( int tamanho, const char * texto);

// Verifica se o CPF lido é válido ou não
int valida_cpf(char * cpf);

// --> FUNÇÕES DO PROGRAMA

Tdata data_hoje();
Tdata_horario data_horario_agora();

// Lê do teclado todos os dados de uma pessoa
void ler_pessoa(Tpessoa * pessoa);

// Processo de cadastramento de cliente
Tcliente * procede_com_cpf(char * cpf);

// Processo de cadastramento de animal (cliente já "logado")
Tanimal * novo_animal(Tcliente * res);

// Modifica um ponteiro com todas as informações de certo animal (por id)
void pega_animal(Tanimal * animal_res);

Tveterinario * cadastra_veterinario();

Tconsulta * marca_consulta(Tanimal * animal_res);

// Printa na tela todos os clientes cadastrados
void printa_clientes();

// Printa na tela todos os veterinários cadastrados
void printa_veterinarios();

// Printa todas as consultas cadastradas associadas a determinado cliente (por ID)
void printa_consultas_por_cliente_id(unsigned int id);

// Printa todas os animais cadastrados
void printa_animais_por_cliente_id(unsigned int id);

// Printa todas as consultas do dia de hoje
void consultas_hoje();



// Globais

// Ponteiros para os arquivos são globais, pois são utilizados por muitas funções.
// Se não fossem globais teriam que ter um parâmetro do arquivo para cada função que os utiliza.
FILE * arq_clientes, * arq_veterinarios, *arq_animais, * arq_consultas;

// Escolhendo buffer sendo global, pois muitas funções também o usam.
char * buffer;

// Controle de erro através de funções diferentes
int err = 0;

int main()
{	
	if ((buffer = calloc(BUFFER_MAX, sizeof(char))) == NULL)
	{
		erro("Erro de alocacao de memoria.\n");
		return -1;
	}
	if ((arq_clientes = fopen(CLIENTES, "ab+")) == NULL || (arq_veterinarios = fopen(VETERINARIOS, "ab+")) == NULL || (arq_consultas = fopen(CONSULTAS, "ab+")) == NULL || (arq_animais = fopen(ANIMAIS, "ab+")) == NULL)
	{
		erro("Erro ao abrir arquivo(s).\n");
		return -2;
	}
	
	printa_arquivo(TELA_INICIAL);
	
	while (!(leitura_linha(50, "Para continuar, digite o proximo comando OU o CPF do proximo cliente: ")))
	{
		if (!strcmp(buffer, "sair"))
		{
			break;
		}
		else if (!strcmp(buffer, "ajuda"))
		{
			printa_arquivo("ajuda.txt");
			continue;
		}
		else if (!strcmp(buffer, "limpar_tela"))
		{
			limpa_tela();
			continue;
		}
		else if (!strcmp(buffer, "mostrar_clientes"))
		{
			printf("\n");
			printa_clientes();
			continue;
		}
		else if (!strcmp(buffer, "mostrar_veterinarios"))
		{
			printa_veterinarios();
			continue;
		}
		else if (!strcmp(buffer, "cadastrar_veterinario"))
		{
			Tveterinario * vet_res;
			vet_res = cadastra_veterinario();
			
			if (vet_res != NULL)
			{
				printf("Veterinario %s cadastrado com sucesso.\n\n", vet_res->dados.nome);
			}
			free(vet_res);
			continue;
		}
		else if (!strcmp(buffer, "consultas_hoje"))
		{
			consultas_hoje();
			continue;
		}
		
		
		if (valida_cpf(buffer))
		{
			Tcliente * res;
			res = procede_com_cpf(buffer);
			
			// Tudo deu certo e o cliente está "logado"
			if (res != NULL)
			{
				printa_consultas_por_cliente_id(res->dados.id);
				printa_animais_por_cliente_id(res->dados.id);
				
				printf("\n\n");
				printa_arquivo(CLIENTE_MENU);
				while (!(leitura_linha(50, "\nSua escolha: ")))
				{
					if (!strcmp(buffer, "sair"))
					{
						break;
					}
					if (!strcmp(buffer, "limpar_tela"))
					{
						limpa_tela();
						continue;
					}
					else if (!strcmp(buffer, "ajuda"))
					{
						printa_arquivo(CLIENTE_AJUDA);
						continue;
					}
					else if (!strcmp(buffer, "adicionar_animal"))
					{
						Tanimal * animal_res;
						animal_res = novo_animal(res);
						free(animal_res);
						continue;
					}
					else if (!strcmp(buffer, "mostrar_animais"))
					{
						printa_animais_por_cliente_id(res->dados.id);
						continue;
					}
					else if (!strcmp(buffer, "marcar_consulta"))
					{
						Tconsulta * consulta_res;
						Tanimal * animal_res;
						if ((animal_res = malloc(sizeof(Tanimal))) == NULL)
						{
							erro("Erro de alocacao de memoria.\n");
							continue;
						}
						memset(animal_res, 0, sizeof(Tanimal));
						
						int animal_id;
						
						printa_animais_por_cliente_id(res->dados.id);
						
						while (leitura_linha(50, "Digite o ID do animal a ser consultado: "));
						animal_id = strtol(buffer, NULL, 0);
						
						animal_res->id = animal_id;
						pega_animal(animal_res);
						
						consulta_res = marca_consulta(animal_res);
						if (consulta_res != NULL)
						{
							printf("\nConsulta marcada com sucesso!\n");
						}
						
						free(animal_res);
						free(consulta_res);
						continue;
					}
					else if (!strcmp(buffer, "mostrar_consultas"))
					{
						printa_consultas_por_cliente_id(res->dados.id);
						continue;
					}
				}
				
				free(res);
			}
		}
		else
			err = -1;
		
		limpa_tela();
		printa_arquivo(TELA_INICIAL);
		
		switch(err)
		{
			case -1:
				printf("CPF invalido.\n\n");
				err = 0;
				break;
		}
		
	}
	
	
	printf("Programa encerrando...");
	free(buffer);
	fclose(arq_clientes);
	fclose(arq_veterinarios);
	fclose(arq_animais);
	fclose(arq_consultas);
	return 0;
}

void printa_arquivo(const char * menu_nome_arq)
{
    FILE * menu_fp;
    char c;

    if (!(menu_fp = fopen(menu_nome_arq, "r")))
    {
        printf("[printa_arquivo] Erro ao abrir arquivo %s de menu.\n", menu_nome_arq);
        return;
    }

    while((c = getc(menu_fp)) != EOF)
    {
        putchar(c);
    }
    fclose(menu_fp);
}

void limpa_tela()
{
#ifdef _WIN32
	for ( int i = 0; i < 100; i++ )
		printf("\n");
#else
	printf("\e[1;1H\e[2J");
#endif
}

void erro(const char * msg)
{
	printa_arquivo(ERRO);
	printf(msg);
}

void reseta_buffer()
{
	memset(buffer, 0, BUFFER_MAX);
}

int leitura_linha(int tamanho, const char * texto)
{
	int ch, extra;

	// Texto de prompt
	if (texto != NULL)
	{
		printf("%s", texto);
		fflush(stdout);
	}

	// Sem entrada
	if (fgets(buffer, tamanho, stdin) == NULL)
		return 1;

	if (buffer[strlen(buffer) - 1] != '\n')
	{
		extra = 0;
		while ((ch = getchar()) != '\n' && (ch != EOF))
			extra = 1;
		return (extra == 1) ? 2 : 0;
	}

	buffer[strlen(buffer) - 1] = '\0';
	return 0;
}

int valida_cpf(char * cpf)
{
    char dig10, dig11;
    int sm, i, r, num, peso;

    if (!strcmp(cpf, "00000000000") || !strcmp(cpf, "11111111111") || !strcmp(cpf, "22222222222") || !strcmp(cpf, "33333333333") || !strcmp(cpf, "44444444444") || !strcmp(cpf, "55555555555") || !strcmp(cpf, "66666666666") || !strcmp(cpf, "77777777777") || !strcmp(cpf, "88888888888") || !strcmp(cpf, "99999999999"))
        return 0;

    sm = 0;
    peso = 10;
    for (i = 0; i < 9; i++)
    {
        num = (int)(cpf[i] - 48);
        sm = sm + (num * peso);
        peso = peso - 1;
    }

    r = 11 - (sm % 11);
    if ((r == 10) || (r == 11))
        dig10 = '0';
    else dig10 = (char)(r + 48);

    sm = 0;
    peso = 11;
    for(i = 0; i < 10; i++)
    {
        num = (int)(cpf[i] - 48);
        sm = sm + (num * peso);
        peso = peso - 1;
    }

    r = 11 - (sm % 11);
    if ((r == 10) || (r == 11))
        dig11 = '0';
    else
        dig11 = (char)(r + 48);

    if ((dig10 == cpf[9]) && (dig11 == cpf[10]))
        return 1;
    else
    {
        return 0;
    }
}

Tdata data_hoje()
{
	Tdata hor;
	time_t tempo;
	struct tm * t;
	
	time(&tempo);
	t = localtime(&tempo);
	
	hor.ano =     t->tm_year;
	hor.mes =     t->tm_mon+1;
	hor.dia =     t->tm_mday;
	
	return hor;
}

Tdata_horario data_horario_agora()
{
	Tdata_horario hor;
	time_t tempo;
	struct tm * t;
	
	time(&tempo);
	t = localtime(&tempo);
	
	hor.ano =	1900 + t->tm_year;
	hor.mes =          t->tm_mon+1;
	hor.dia =          t->tm_mday;
	hor.horas =        t->tm_hour;
	hor.minutos =      t->tm_min;
	
	return hor;
}

void ler_pessoa(Tpessoa * pessoa)
{	
	printf("CPF: %s\n", pessoa->cpf);
	
	while (leitura_linha(BUFFER_MAX, "Digite o nome: "));
	strcpy(pessoa->nome, buffer);
	
	while (leitura_linha(12, "Digite o telefone: "));
	strcpy(pessoa->telefone, buffer);

	// Lê no formato (DD/MM/AAAA)
	while (leitura_linha(3, "Digite o dia de nascimento (1-31): "));
	if (buffer[0] == '0')
		pessoa->data_de_nascimento.dia = buffer[1] - 48;
	else
		pessoa->data_de_nascimento.dia = strtol(buffer, NULL, 0);

	while (leitura_linha(3, "Digite o mes de nascimento (1-12): "));
	if (buffer[0] == '0')
		pessoa->data_de_nascimento.mes = buffer[1] - 48;
	else
		pessoa->data_de_nascimento.mes = strtol(buffer, NULL, 0);


	while (leitura_linha(5, "Digite o ano de nascimento: "));
	pessoa->data_de_nascimento.ano = strtol(buffer, NULL, 0);
}

// Retornando um tipo Tcliente pois queremos um certo tipo de persistencia no sistema, como um login
Tcliente * procede_com_cpf(char * cpf)
{
	Tcliente cliente_leitura = {0}, *res;
	int existe = 0;
	
	if ((res = malloc(sizeof(Tcliente))) == NULL)
	{
		erro("Erro de alocacao de memoria.\n");
		return NULL;
	}
	
	memset(res, 0, sizeof(Tcliente));
	
	// PRIMEIRO: devemos verificar se a pessoa com CPF cpf já é cliente.
	
	rewind(arq_clientes);
	
	while ((fread(&cliente_leitura, sizeof(Tcliente), 1, arq_clientes)) == 1)
	{
		if (!strcmp(cliente_leitura.dados.cpf, cpf))
			existe = 1;
	}
	
	strcpy(res->dados.cpf, cpf);
	
	
	// Não é cadastrado ainda
	if (!existe)
	{
		if (ftell(arq_clientes) == 0)
		{
			res->dados.id = 0;
		}
		// Precisamos obter o id do cliente anterior para cadastrar o novo
		else
		{
			if (!fseek(arq_clientes, - (long) sizeof(Tcliente), SEEK_END))
			{
				if ((fread(&cliente_leitura, sizeof(Tcliente), 1, arq_clientes)) == 1)
				{
					res->dados.id = cliente_leitura.dados.id + 1;
				}
				else
				{
					erro("Erro ao ler arquivo.\n");
				}
			}
			else
			{
				erro("Erro ao posicionar ponteiro de arquivo.\n");
			}
		}
		
		// Pede todas as informações
		ler_pessoa(&res->dados);
		
		fseek(arq_clientes, 0, SEEK_END);
		fwrite(res, sizeof(Tcliente), 1, arq_clientes);
		
		printf("Cliente %s cadastrado com sucesso!\n", res->dados.nome);
	}
	// Já é cadastrado, vamos "fazer login"
	else
	{
		while ((fread(&cliente_leitura, sizeof(Tcliente), 1, arq_clientes)) == 1)
		{
			if (!strcmp(cliente_leitura.dados.cpf, cpf))
				break;
		}
		
		res->dados = cliente_leitura.dados;
	}
	
	return res;
}

/* Tanimal.tipo
 * 'a' - ave
 * 'g' - gato
 * 'c' - cão
 * 't' - tartaruga
 * 'j' - jabuti
 * 'h' - hamster
 * 's' - silvestre
*/
Tanimal * novo_animal(Tcliente * res)
{
	Tanimal animal_leitura = {0};
	Tanimal * animal_res;
	
	
	if ((animal_res = malloc(sizeof(Tanimal))) == NULL)
	{
		erro("Erro de alocacao de memoria.\n");
		return NULL;
	}
	
	memset(animal_res, 0, sizeof(Tanimal));
	
	fseek(arq_animais, 0L, SEEK_END);
	
	while (leitura_linha(BUFFER_MAX, "\nDigite o nome do animal: "));
	strcpy(animal_res->nome, buffer);
	
	printa_arquivo(ANIMAIS_TIPOS);
	while (leitura_linha(2, "Digite o tipo do animal: "));
	animal_res->tipo = buffer[0];
	
	animal_res->cliente_id = res->dados.id;
	if (ftell(arq_animais) == 0)
	{
		animal_res->id = 0;
	}
	else
	{
		if (!fseek(arq_animais, - (long) sizeof(Tanimal), SEEK_END))
		{
			if ((fread(&animal_leitura, sizeof(Tanimal), 1, arq_animais)) == 1)
			{
				animal_res->id = animal_leitura.id + 1;
			}
			else
			{
				erro("Erro ao ler arquivo.\n");
				return NULL;
			}
		}
		else
		{
			erro("Erro ao posicionar ponteiro de arquivo.\n");
			return NULL;
		}
	}
	
	animal_res->ultima_consulta.ano = 0;
	animal_res->ultima_consulta.mes = 0;
	animal_res->ultima_consulta.dia = 0;
	animal_res->ultima_consulta.horas = 0;
	animal_res->ultima_consulta.minutos = 0;
	
	fseek(arq_animais, 0L, SEEK_END);
	
	if (fwrite(animal_res, sizeof(Tanimal), 1, arq_animais) == 1)
	{
		return animal_res;
	}
	else
	{
		erro("Erro ao escrever no arquivo.\n");
		return NULL;
	}
}

void pega_animal(Tanimal * animal_res)
{
	Tanimal animal_leitura = {0};
	if (animal_res == NULL)
	{
		erro("Ponteiro nulo.\n");
		return;
	}
	
	rewind(arq_animais);
	
	while (fread(&animal_leitura, sizeof(Tanimal), 1, arq_animais) == 1)
	{
		if (animal_leitura.id == animal_res->id)
			break;
	}
	
	*animal_res = animal_leitura;
}

Tveterinario * cadastra_veterinario()
{
	Tveterinario veterinario_leitura = {0}, *vet;
	int volta = 0;
	
	if ((vet = malloc(sizeof(Tveterinario))) == NULL)
	{
		erro("Erro de alocacao de memoria.\n");
		return NULL;
	}
	
	memset(vet, 0, sizeof(Tveterinario));
	
	rewind(arq_veterinarios);
	
	while (!leitura_linha(50, "Digite o CPF do novo veterinario: ") && !volta)
	{
		if (!valida_cpf(buffer))
		{
			continue;
		}
		volta = 0;
		while (fread(&veterinario_leitura, sizeof(Tveterinario), 1, arq_veterinarios) == 1)
		{
			if (!strcmp(veterinario_leitura.dados.cpf, buffer))
			{
				printf("Veterinario ja cadastrado.\n");
				volta = 1;
				break;
			}
		}
		break;
	}
	
	strcpy(vet->dados.cpf, buffer);

	fseek(arq_veterinarios, 0, SEEK_END);
	
	// Primeiro veterinário a ser cadastrado
	if (ftell(arq_veterinarios) == 0)
	{
		vet->dados.id = 0;
	}
	else
	{
		if (!fseek(arq_veterinarios, - (long) sizeof(Tveterinario), SEEK_END))
		{
			if (fread(&veterinario_leitura, sizeof(Tveterinario), 1, arq_veterinarios) == 1)
			{
				vet->dados.id = veterinario_leitura.dados.id + 1;
			}
			else
			{
				erro("Erro ao ler arquivo.\n");
				return NULL;
			}
		}
		else
		{
			erro("Erro ao posicionar ponteiro de arquivo.\n");
			return NULL;
		}
	}
	
	ler_pessoa(&vet->dados);
	
	while (leitura_linha(10, "Digite o salario do novo veterinario: "));
	vet->salario = strtod(buffer, NULL);
	
	while (leitura_linha(10, "Digite o valor da consulta do novo veterinario: "));
	vet->valor_consulta = strtod(buffer, NULL);
	
	fseek(arq_veterinarios, 0, SEEK_END);
	
	if (fwrite(vet, sizeof(Tveterinario), 1, arq_veterinarios) == 1)
	{
		return vet;
	}
	
	return NULL;
}

Tconsulta * marca_consulta(Tanimal * animal_res)
{
	Tconsulta *consulta_res;
	Tveterinario vet = {0};
	
	if ((consulta_res = malloc(sizeof(Tconsulta))) == NULL)
	{
		erro("Erro de alocacao de memoria.\n");
		return NULL;
	}
	
	memset(consulta_res, 0, sizeof(Tconsulta));
	
	rewind(arq_consultas);
	
	printa_arquivo(CONSULTA_MOTIVOS);
	
	leitura_linha(4, "Digite o motivo da consulta: ");
	strcpy(consulta_res->motivo, buffer);
	
	printa_veterinarios();
	leitura_linha(50, "Selecione um veterinario para a consulta (id): ");
	consulta_res->vet_id = strtol(buffer, NULL, 0);
	
	rewind(arq_veterinarios);
	while (fread(&vet, sizeof(Tveterinario), 1, arq_veterinarios) == 1)
	{
		if (vet.dados.id == consulta_res->vet_id)
			break;
	}
	
	consulta_res->valor = vet.valor_consulta;
	consulta_res->cliente_id = animal_res->cliente_id;
	consulta_res->animal_id = animal_res->id;
	
	leitura_linha(3, "Digite o dia desejado para a consulta: ");
	consulta_res->horario.dia = 10 * (buffer[0] - 48) + (buffer[1] - 48);
	
	
	leitura_linha(3, "Digite o mes desejado para a consulta: ");
	consulta_res->horario.mes = 10 * (buffer[0] - 48) + (buffer[1] - 48);
	
	leitura_linha(5, "Digite o ano desejado para a consulta: ");
	consulta_res->horario.ano = (1000 * (buffer[0] - 48)) + (100 * (buffer[1] - 48)) + (10 * (buffer[2] - 48)) + (buffer[1] - 48) + 8;
	
	leitura_linha(5, "Digite o horario desejado para a consulta (horas): ");
	consulta_res->horario.horas = 10 * (buffer[0] - 48) + (buffer[1] - 48);
	
	
	leitura_linha(5, "Digite o horario desejado para a consulta (minutos): ");
	consulta_res->horario.minutos = 10 * (buffer[0] - 48) + (buffer[1] - 48);
	
	rewind(arq_consultas);
	
	if (fwrite(consulta_res, sizeof(Tconsulta), 1, arq_consultas) == 1)
	{
		return consulta_res;
	}
	
	return NULL;
	
	
}

void printa_clientes()
{
	Tcliente cliente = {0};
	rewind(arq_clientes);
	
	while ((fread(&cliente, sizeof(Tcliente), 1, arq_clientes)) == 1)
	{
		printf("cliente.id = %u\n", cliente.dados.id);
		printf("cliente.cpf = %s\n", cliente.dados.cpf);
		printf("cliente.nome = %s\n", cliente.dados.nome);
		printf("cliente.telefone = %s\n", cliente.dados.telefone);
		printf("cliente.data_de_nascimento.dia = %d\n", cliente.dados.data_de_nascimento.dia);
		printf("cliente.data_de_nascimento.mes = %d\n", cliente.dados.data_de_nascimento.mes);
		printf("cliente.data_de_nascimento.ano = %d\n", cliente.dados.data_de_nascimento.ano);
		
		printf("\n\n\n");
	}
}

void printa_veterinarios()
{
	Tveterinario veterinario = {0};
	rewind(arq_veterinarios);
	int cont = 0;
	
	printf("\n[VETERINARIOS DISPONIVEIS]\n");
	while ((fread(&veterinario, sizeof(Tveterinario), 1, arq_veterinarios)) == 1)
	{
		cont++;
		
		// Exibimos apenas dados que interessam o cliente, por isso não mostramos salário 
		
		printf("veterinario.id = %u\n", veterinario.dados.id);
		//printf("veterinario.cpf = %s\n", veterinario.dados.cpf);
		printf("veterinario.nome = %s\n", veterinario.dados.nome);
		printf("veterinario.telefone = %s\n", veterinario.dados.telefone);
		//printf("veterinario.data_de_nascimento.dia = %d\n", veterinario.dados.data_de_nascimento.dia);
		//printf("veterinario.data_de_nascimento.mes = %d\n", veterinario.dados.data_de_nascimento.mes);
		//printf("veterinario.data_de_nascimento.ano = %d\n", veterinario.dados.data_de_nascimento.ano);
		printf("veterinario.valor_consulta = %.2f\n", veterinario.valor_consulta);
			
		printf("\n\n\n");
	}
	
	if (cont == 1)
	{
		printf("Existe %d veterinario(a) disponivel.\n\n", cont);
	}
	else if (cont == 0)
	{
		printf("Nenhum(a) veterinario(a) foi cadastrado ainda.\n\n");
	}
	else
	{
		printf("Existem %d veterinarios(as) disponiveis.\n\n", cont);
	}
}

void printa_consultas_por_cliente_id(unsigned int id)
{
	Tconsulta consulta_leitura = {0};
	rewind(arq_consultas);
	int cont = 0;
	
	printf("[CONSULTAS DO CLIENTE %u]\n", id);
	while ((fread(&consulta_leitura, sizeof(Tconsulta), 1, arq_consultas)) == 1)
	{
		if (consulta_leitura.cliente_id == id)
		{
			cont++;
			printf("consulta.animal_id = %u\n", consulta_leitura.animal_id);
			printf("consulta.vet_id = %u\n", consulta_leitura.vet_id);
			printf("consulta.motivo = %s\n", consulta_leitura.motivo);
			printf("consulta.valor = %.2f\n", consulta_leitura.valor);
			printf("consulta.horario = %d/%d/%d %d:%d\n\n", consulta_leitura.horario.dia, consulta_leitura.horario.mes, consulta_leitura.horario.ano, consulta_leitura.horario.horas, consulta_leitura.horario.minutos);
		}
	}
	if (cont == 0)
		printf("Nenhuma consulta marcada encontrada para nenhum animal do cliente.\n");
	else
		printf("Total de %d consultas encontradas para o cliente.\n", cont);
}

void printa_animais_por_cliente_id(unsigned int id)
{
	Tanimal animal_leitura = {0};
	rewind(arq_animais);
	int cont = 0;
	
	printf("\n[ANIMAIS CADASTRADOS DO CLIENTE %u]\n", id);
	while ((fread(&animal_leitura, sizeof(Tanimal), 1, arq_animais)) == 1)
	{
		if (animal_leitura.cliente_id == id)
		{
			cont++;
			printf("animal.id = %u\n", animal_leitura.id);
			printf("animal.nome = %s\n", animal_leitura.nome);
			switch(animal_leitura.tipo)
			{
				case 'a':
					strcpy(buffer, "ave");
					break;
				case 'g':
					strcpy(buffer, "gato");
					break;
				case 'c':
					strcpy(buffer, "cao");
					break;
				case 't':
					strcpy(buffer, "tartaruga");
					break;
				case 'j':
					strcpy(buffer, "jabuti");
					break;
				case 'h':
					strcpy(buffer, "hamster");
					break;
				case 's':
					strcpy(buffer, "animal silvestre");
					break;
			}
			printf("animal.tipo = %s\n", buffer);
			printf("animal_leitura.ultima_consulta = %d/%d/%d %d:%d\n\n", animal_leitura.ultima_consulta.dia, animal_leitura.ultima_consulta.mes, animal_leitura.ultima_consulta.ano, animal_leitura.ultima_consulta.horas, animal_leitura.ultima_consulta.minutos);
		}
	}
	
	if (cont == 0)
			printf("Nenhum animal cadastrado encontrado do cliente.\n");
		else
			printf("Total de %d animais cadastrados encontradas do cliente.\n", cont);
}

void consultas_hoje()
{
	Tconsulta consulta_leitura = {0};
	time_t agora;
	struct tm * agora_tm;
	int cont = 0;
	
	time(&agora);
	agora_tm = localtime(&agora);
	
	rewind(arq_consultas);
	
	printf("\n[CONSULTAS MARCADAS PARA HOJE]\n");
	while (fread(&consulta_leitura, sizeof(Tconsulta), 1, arq_consultas) == 1)
	{
		if (consulta_leitura.horario.ano == agora_tm->tm_year && consulta_leitura.horario.mes == agora_tm->tm_mon && consulta_leitura.horario.dia == agora_tm->tm_mday)
		{
			cont++;
			printf("consulta.animal_id = %u\n", consulta_leitura.animal_id);
			printf("consulta.cliente_id = %u\n", consulta_leitura.cliente_id);
			printf("consulta.vet_id = %u\n", consulta_leitura.vet_id);
			printf("consulta.animal_id = %s\n", consulta_leitura.motivo);
			printf("consulta.valor = %.2f\n", consulta_leitura.valor);
			printf("consulta.horario = %d:%d\n", consulta_leitura.horario.horas, consulta_leitura.horario.minutos);
		}
	}
	
	if (cont)
	{
		printf("Total de %d consultas marcadas para hoje.\n\n", cont);
	}
	else
	{
		printf("Nenhuma consulta marcada para hoje.\n\n");
	}
}
