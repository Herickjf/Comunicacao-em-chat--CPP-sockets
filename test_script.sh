#!/bin/bash

echo " Iniciando teste de chat com múltiplos clientes..."
echo "=============================================="

# Função para matar processos ao sair
cleanup() {
    echo ""
    echo " Limpando processos..."
    kill $SERVER_PID 2>/dev/null
    pkill -f "./bin/client" 2>/dev/null
    exit 0
}

trap cleanup SIGINT

# Iniciar servidor em background
echo "  Iniciando servidor..."
./bin/server &
SERVER_PID=$!

# Aguardar servidor iniciar
echo " Aguardando servidor iniciar (3 segundos)..."
sleep 3

# Verificar se servidor está rodando
if ! kill -0 $SERVER_PID 2>/dev/null; then
    echo " Erro: Servidor não iniciou corretamente"
    exit 1
fi

echo " Servidor iniciado com PID: $SERVER_PID"

# Função para iniciar cliente automático
start_automated_client() {
    local name=$1
    local group=$2
    local messages=("${@:3}")
    
    echo " Iniciando cliente: $name (Grupo $group)"
    
    # Criar pipe para enviar comandos automáticos
    (
        # Aguardar prompt inicial
        sleep 2
        
        # Escolher grupo
        echo "$group"
        
        # Aguardar entrar no grupo
        sleep 1
        
        # Enviar mensagens
        for msg in "${messages[@]}"; do
            echo "$msg"
            sleep 2
        done
        
        # Sair
        echo "/quit"
        
    ) | ./bin/client 127.0.0.1 8080 "$name" > "log_$name.txt" 2>&1 &
}

# Clientes para Grupo 1 (Geral)
start_automated_client "Alice" 1 "Olá pessoal!" "Como vocês estão?" "Alguém no Grupo 1?"
start_automated_client "Bob" 1 "Oi Alice!" "Estou aqui também!" "Tudo bem por aqui!"

# Clientes para Grupo 2 (Desenvolvimento)
start_automated_client "Carlos" 2 "Pessoal do desenvolvimento" "Alguém trabalhando no projeto?" "Vamos sincronizar?"
start_automated_client "Diana" 2 "Oi Carlos" "Estou no código agora" "Preciso de ajuda com threads"

# Cliente para Grupo 3 (Suporte)
start_automated_client "Eva" 3 "Time de suporte" "Algum chamado aberto?" "Preciso de ajuda"

echo ""
echo "Clientes em execução:"
echo "   Grupo 1 (Geral): Alice, Bob"
echo "   Grupo 2 (Desenvolvimento): Carlos, Diana" 
echo "   Grupo 3 (Suporte): Eva"
echo ""
echo " Logs sendo salvos em: log_Alice.txt, log_Bob.txt, etc."
echo ""
echo "Teste rodando por 30 segundos..."
echo "   Pressione Ctrl+C para parar antes"

# Aguardar um tempo para ver a interação
sleep 30

echo ""
echo "Tempo de teste concluído. Finalizando..."

# Parar processos
cleanup