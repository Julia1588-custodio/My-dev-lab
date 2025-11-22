#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LIVROS 200
#define MAX_USUARIOS 200
#define MAX_EMPRESTIMOS 500

// ---------- STRUCTS ----------

typedef struct {
    int dia, mes, ano;
} Data;

typedef struct {
    int codigo;
    char titulo[100];
    char autor[80];
    char editora[60];
    int ano;
    int exemplares;
    int disponivel;
} Livro;

typedef struct {
    int matricula;
    char nome[100];
    char curso[50];
    char telefone[15];
    Data cadastro;
} Usuario;

typedef struct {
    int codigoEmp;
    int matricula;
    int codLivro;
    Data dataEmp;
    Data dataDev;
    int status; 
} Emprestimo;

// ---------- VETORES ----------

Livro livros[MAX_LIVROS];
Usuario usuarios[MAX_USUARIOS];
Emprestimo emp[MAX_EMPRESTIMOS];

int totLivros = 0, totUsuarios = 0, totEmp = 0;

// ---------- FUNÇÕES ----------

void pausa() {
    printf("\nPressione ENTER para continuar...");
    getchar();
}

Data lerDataHoje() {
    Data d;
    d.dia = 10;
    d.mes = 11;
    d.ano = 2025;
    return d; 
}

Data somar7Dias(Data d) {
    d.dia += 7;
    if (d.dia > 30) {
        d.dia -= 30;
        d.mes++;
    }
    return d;
}

// ---------- SALVAMENTO E CARREGAMENTO ----------

void salvarLivros() {
    FILE *f = fopen("livros.txt", "w");
    for (int i = 0; i < totLivros; i++) {
        fprintf(f, "%d;%s;%s;%s;%d;%d;%d\n",
            livros[i].codigo,
            livros[i].titulo,
            livros[i].autor,
            livros[i].editora,
            livros[i].ano,
            livros[i].exemplares,
            livros[i].disponivel
        );
    }
    fclose(f);
}

void carregarLivros() {
    FILE *f = fopen("livros.txt", "r");
    if (!f) return;
    while (!feof(f)) {
        Livro L;
        fscanf(f, "%d;%[^;];%[^;];%[^;];%d;%d;%d\n",
            &L.codigo, L.titulo, L.autor, L.editora,
            &L.ano, &L.exemplares, &L.disponivel);
        livros[totLivros++] = L;
    }
    fclose(f);
}

void salvarUsuarios() {
    FILE *f = fopen("usuarios.txt", "w");
    for (int i = 0; i < totUsuarios; i++) {
        fprintf(f, "%d;%s;%s;%s;%d/%d/%d\n",
            usuarios[i].matricula,
            usuarios[i].nome,
            usuarios[i].curso,
            usuarios[i].telefone,
            usuarios[i].cadastro.dia,
            usuarios[i].cadastro.mes,
            usuarios[i].cadastro.ano
        );
    }
    fclose(f);
}

void carregarUsuarios() {
    FILE *f = fopen("usuarios.txt", "r");
    if (!f) return;
    while (!feof(f)) {
        Usuario U;
        fscanf(f, "%d;%[^;];%[^;];%[^;];%d/%d/%d\n",
            &U.matricula, U.nome, U.curso, U.telefone,
            &U.cadastro.dia, &U.cadastro.mes, &U.cadastro.ano);
        usuarios[totUsuarios++] = U;
    }
    fclose(f);
}

void salvarEmprestimos() {
    FILE *f = fopen("emprestimos.txt", "w");
    for (int i = 0; i < totEmp; i++) {
        fprintf(f, "%d;%d;%d;%d/%d/%d;%d/%d/%d;%d\n",
            emp[i].codigoEmp,
            emp[i].matricula,
            emp[i].codLivro,
            emp[i].dataEmp.dia, emp[i].dataEmp.mes, emp[i].dataEmp.ano,
            emp[i].dataDev.dia, emp[i].dataDev.mes, emp[i].dataDev.ano,
            emp[i].status
        );
    }
    fclose(f);
}

void carregarEmprestimos() {
    FILE *f = fopen("emprestimos.txt", "r");
    if (!f) return;
    while (!feof(f)) {
        Emprestimo E;
        fscanf(f, "%d;%d;%d;%d/%d/%d;%d/%d/%d;%d\n",
            &E.codigoEmp, &E.matricula, &E.codLivro,
            &E.dataEmp.dia, &E.dataEmp.mes, &E.dataEmp.ano,
            &E.dataDev.dia, &E.dataDev.mes, &E.dataDev.ano,
            &E.status
        );
        emp[totEmp++] = E;
    }
    fclose(f);
}

// ---------- CADASTROS DOS LIVROS ----------

void cadastrarLivro() {
    Livro L;
    printf("\n--- Cadastro de Livro ---\n");
    printf("Codigo: ");
    scanf("%d", &L.codigo); getchar();

    printf("Titulo: ");
    fgets(L.titulo, 100, stdin);
    strtok(L.titulo, "\n");

    printf("Autor: ");
    fgets(L.autor, 80, stdin);
    strtok(L.autor, "\n");

    printf("Editora: ");
    fgets(L.editora, 60, stdin);
    strtok(L.editora, "\n");

    printf("Ano: ");
    scanf("%d", &L.ano);

    printf("Qtd exemplares: ");
    scanf("%d", &L.exemplares);

    L.disponivel = L.exemplares;

    livros[totLivros++] = L;
    salvarLivros();
    printf("Livro cadastrado!\n");
}

void cadastrarUsuario() {
    Usuario U;
    printf("\n--- Cadastro de Usuario ---\n");
    printf("Matricula: ");
    scanf("%d", &U.matricula); getchar();

    printf("Nome: ");
    fgets(U.nome, 100, stdin);
    strtok(U.nome, "\n");

    printf("Curso: ");
    fgets(U.curso, 50, stdin);
    strtok(U.curso, "\n");

    printf("Telefone: ");
    fgets(U.telefone, 15, stdin);
    strtok(U.telefone, "\n");

    U.cadastro = lerDataHoje();

    usuarios[totUsuarios++] = U;
    salvarUsuarios();
    printf("Usuario cadastrado!\n");
}

// ---------- EMPRÉSTIMOS ----------

void realizarEmprestimo() {
    Emprestimo E;
    printf("\n--- Realizar Emprestimo ---\n");

    printf("Matricula: ");
    scanf("%d", &E.matricula);

    printf("Codigo do livro: ");
    scanf("%d", &E.codLivro);

    int posL = -1;
    for (int i = 0; i < totLivros; i++) {
        if (livros[i].codigo == E.codLivro)
            posL = i;
    }

    if (posL == -1) {
        printf("Livro nao encontrado.\n");
        return;
    }

    if (livros[posL].disponivel == 0) {
        printf("Nao ha exemplares disponiveis.\n");
        return;
    }

    E.codigoEmp = totEmp + 1;
    E.dataEmp = lerDataHoje();
    E.dataDev = somar7Dias(E.dataEmp);
    E.status = 1;

    emp[totEmp++] = E;

    livros[posL].disponivel--;

    salvarLivros();
    salvarEmprestimos();

    printf("Emprestimo realizado com sucesso!\n");
}

void devolverLivro() {
    int cod;
    printf("\n--- Devolver Livro ---\n");
    printf("Codigo do emprestimo: ");
    scanf("%d", &cod);

    for (int i = 0; i < totEmp; i++) {
        if (emp[i].codigoEmp == cod && emp[i].status == 1) {
            emp[i].status = 0;

            int posL = -1;
            for (int j = 0; j < totLivros; j++)
                if (livros[j].codigo == emp[i].codLivro)
                    posL = j;

            if (posL != -1)
                livros[posL].disponivel++;

            salvarLivros();
            salvarEmprestimos();

            printf("Devolucao registrada!\n");
            return;
        }
    }

    printf("Emprestimo nao encontrado.\n");
}

// ---------- MENU ----------

int menu() {
    int op;
    printf("\n======= SISTEMA BIBLIOTECA =======\n");
    printf("1 - Cadastrar Livro\n");
    printf("2 - Cadastrar Usuario\n");
    printf("3 - Realizar Emprestimo\n");
    printf("4 - Devolver Livro\n");
    printf("0 - Sair\n");
    printf("Escolha: ");
    scanf("%d", &op); getchar();
    return op;
}

int main() {
    carregarLivros();
    carregarUsuarios();
    carregarEmprestimos();

    int op;
    do {
        op = menu();
        switch (op) {
            case 1: cadastrarLivro(); break;
            case 2: cadastrarUsuario(); break;
            case 3: realizarEmprestimo(); break;
            case 4: devolverLivro(); break;
            case 0: printf("Saindo...\n"); break;
            default: printf("Opcao invalida.\n");
        }
        pausa();
    } while (op != 0);

    return 0;
}
