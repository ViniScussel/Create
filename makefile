# Nome do executável final
TARGET = tasker

# Compilador C++
CXX = g++

# Flags de compilação:
# -Wall: Habilita todos os avisos (melhor prática)
# -std=c++17: Usa o padrão C++17 (ou superior, ex: c++20)
CXXFLAGS = -Wall -std=c++17

# Diretórios
SRCDIR = codebase
OBJDIR = obj

# Arquivos de origem (adicione todos os seus .cpp aqui)
SOURCES = main.cpp \
          $(SRCDIR)/create.cpp \

# Arquivos objeto (.o) gerados a partir dos SOURCES
OBJECTS = $(patsubst %.cpp, $(OBJDIR)/%.o, $(SOURCES))

# ------------------------------------------------------------------

# Regra principal: 'make' ou 'make all'
all: $(TARGET)

# 1. Cria o diretório de objetos se ele não existir
$(OBJDIR):
	mkdir -p $(OBJDIR)/$(SRCDIR)

# 2. Regra para linkagem: Constrói o executável a partir dos objetos
$(TARGET): $(OBJDIR) $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(TARGET)

# 3. Regra para compilação: Transforma .cpp em .o
# A regra abaixo lida com main.cpp (direto na raiz)
$(OBJDIR)/%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# A regra abaixo lida com os arquivos dentro de codebase/
$(OBJDIR)/$(SRCDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 4. Regra de limpeza: Remove arquivos gerados
.PHONY: clean install

clean:
	rm -rf $(OBJDIR) $(TARGET)

# 5. Regra de instalação (Para mover para o PATH, como discutido)
install: $(TARGET)
	# Garante que seja executavel
	chmod +x $(TARGET)
	# Move para um local no PATH (pode precisar de 'sudo')
	sudo mv $(TARGET) /usr/local/bin/