import pygame
import csv
import math
import time

# Função para ler a linha atual do CSV
def ler_linha_csv(arquivo, linha_atual):
    with open(arquivo, 'r') as f:
        leitor = csv.reader(f, delimiter=';')
        for i, linha in enumerate(leitor):
            if i == linha_atual:
                return linha
    return None

# Função para desenhar os triângulos na tela com as bolinhas dinâmicas
def desenhar_triangulos_com_bolinhas(screen, bolinhas_pos, cores_pos):
    for indice in bolinhas_pos.keys():
        cores = cores_pos.get(indice, [])
        bolinhas = bolinhas_pos[indice]
        
        # Desenhar linhas conectando as bolinhas na ordem especificada
        if len(bolinhas) == 9:
            ordem = [0, 1, 2, 8, 7, 6, 5, 4, 3]
            pontos_triangulo = [bolinhas[idx] for idx in ordem]
            pygame.draw.lines(screen, (255, 255, 255), True, pontos_triangulo, 1)  # Branco, espessura 1

        # Desenhar cada bolinha com sua cor correspondente
        for bolinha, cor in zip(bolinhas, cores):
            try:
                # Verificar se a cor é válida
                if all(0 <= valor <= 255 for valor in cor):
                    pygame.draw.circle(screen, cor, (int(bolinha[0]), int(bolinha[1])), 5)
                else:
                    raise ValueError
            except (TypeError, ValueError):
                print(f"Cor inválida {cor} para a bolinha na posição {bolinha}. Usando cor padrão branca.")
                pygame.draw.circle(screen, (255, 255, 255), (int(bolinha[0]), int(bolinha[1])), 5)

# Função para preparar bolinhas e cores
def preparar_bolinhas_cores(dados_csv):
    bolinhas_pos = {}
    cores_pos = {}
    distancia_inicial = 10

    for linha in dados_csv:
        try:
            indice = int(linha[0])
            cores = [tuple(map(int, linha[j:j+3])) for j in range(1, len(linha), 3)]
            num_bolinhas = len(cores)
            num_bolinhas_lado = int(math.ceil(num_bolinhas / 3))

            if indice not in bolinhas_pos:
                bolinhas = []
                for lado in range(num_bolinhas_lado):
                    if lado == 0:
                        for bolinha in range(3):
                            if lado * 3 + bolinha < num_bolinhas:
                                bolinha_x = 200 + indice * 170 + bolinha * 50 + distancia_inicial
                                bolinha_y = 300 + lado * 50
                                bolinhas.append((bolinha_x, bolinha_y))
                    if lado == 1:
                        for bolinha in range(3):
                            if lado * 3 + bolinha < num_bolinhas:
                                bolinha_x = 170 + indice * 170 + bolinha * 30 + distancia_inicial
                                bolinha_y = 320 - lado * 50 - bolinha * 40
                                bolinhas.append((bolinha_x, bolinha_y))
                    if lado == 2:
                        for bolinha in range(3):
                            if lado * 3 + bolinha < num_bolinhas:
                                bolinha_x = 270 + indice * 170 + bolinha * 30 + distancia_inicial
                                bolinha_y = 290 - lado * 50 + bolinha * 40
                                bolinhas.append((bolinha_x, bolinha_y))
                bolinhas_pos[indice] = bolinhas
                cores_pos[indice] = cores
                distancia_inicial += 50
            else:
                cores_pos[indice] = cores  # Atualizar as cores se o índice já existir
        except ValueError:
            print(f"Erro ao processar a linha: {linha}. Pulando esta linha.")

    return bolinhas_pos, cores_pos

# Função para rotacionar 180 graus
def rotacionar_180(bolinhas):
    centro_x = sum(b[0] for b in bolinhas) / len(bolinhas)
    centro_y = sum(b[1] for b in bolinhas) / len(bolinhas)
    return [(2 * centro_x - b[0], 2 * centro_y - b[1]) for b in bolinhas]

# Inicializar o Pygame
pygame.init()

# Definir as dimensões da tela
largura_tela = 1300
altura_tela = 800
tela = pygame.display.set_mode((largura_tela, altura_tela))
pygame.display.set_caption('Desenho de Triângulos com Bolinhas Dinâmicas')

# Carregar dados do CSV
arquivo_csv = 'C:\\Users\\Hiuri\\Desktop\\Desenvolvimento\\input.csv'
arquivo_csv = 'input.csv'
dados_csv = list(csv.reader(open(arquivo_csv, 'r'), delimiter=';'))

# Inicializar posição inicial das bolinhas e cores
bolinhas_pos, cores_pos = preparar_bolinhas_cores(dados_csv)
print(bolinhas_pos)

# Configuração do temporizador
intervalo_troca_cores = 0.03  # segundos
tempo_atual_ultimo_tempo = 0
linha_atual = 0

# Loop principal
rodando = True
selecionado = None
offset_x, offset_y = 0, 0

while rodando:
    tempo_atual = time.time()

    if tempo_atual - tempo_atual_ultimo_tempo >= intervalo_troca_cores:
        linha = ler_linha_csv(arquivo_csv, linha_atual)
        if linha:
            indice = int(linha[0])
            cores = [tuple(map(int, linha[j:j+3])) for j in range(1, len(linha), 3)]
            cores_pos[indice] = cores
        linha_atual = (linha_atual + 1) % len(dados_csv)  # Atualiza para a próxima linha
        tempo_atual_ultimo_tempo = tempo_atual

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            rodando = False
        elif event.type == pygame.MOUSEBUTTONDOWN:
            pos = pygame.mouse.get_pos()
            for tri, bolinhas in bolinhas_pos.items():
                for bolinha in bolinhas:
                    distancia = math.sqrt((bolinha[0] - pos[0]) ** 2 + (bolinha[1] - pos[1]) ** 2)
                    if distancia <= 5:
                        selecionado = tri
                        offset_x = bolinha[0] - pos[0]
                        offset_y = bolinha[1] - pos[1]
                        break
                if selecionado:
                    break
        elif event.type == pygame.MOUSEBUTTONUP:
            selecionado = None
        elif event.type == pygame.MOUSEMOTION:
            if selecionado is not None:
                pos = pygame.mouse.get_pos()
                novas_posicoes = [(pos[0] + offset_x + dx, pos[1] + offset_y + dy)
                                  for dx, dy in [(b[0] - bolinhas_pos[selecionado][0][0], 
                                                  b[1] - bolinhas_pos[selecionado][0][1])
                                                 for b in bolinhas_pos[selecionado]]]
                bolinhas_pos[selecionado] = novas_posicoes
        elif event.type == pygame.KEYDOWN:
            if event.key == pygame.K_SPACE and selecionado is not None:
                bolinhas_pos[selecionado] = rotacionar_180(bolinhas_pos[selecionado])

    tela.fill((0, 0, 0))  # Fundo preto 

    # Desenhar os triângulos com as bolinhas dinâmicas
    desenhar_triangulos_com_bolinhas(tela, bolinhas_pos, cores_pos)

    pygame.display.flip()

pygame.quit()
