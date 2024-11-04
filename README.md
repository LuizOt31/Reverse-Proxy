# Reverse-Proxy

Esse trabalho foi realizado para o processo seletivo do PATOS.UFSCAR, um grupo de Open Source da Universidade Federal de São Carlos.

O trabalho foi feito totalmente em C, construindo um reverse proxy, sistema que se conecta com o cliente e administra sua interação com o server de que ele faz parte. Por isso, neste repositório há 3 arquivos, dois para representarem um cliente fazendo uma requisição e um server que está esperando ela, e por fim um reverse proxy que faz o intermédio dessa interação (perceba que os nomes dos arquivos fazem referência ao que eles são).

## Como rodar?

Simples! Compile todos os 3 e execute-os na ordem a seguir: server, reverse proxy, cliente. Suas conexões serão estabelecidas automaticamente e você poderá interagir (da perspectiva do cliente) mandando mensagens para o server, onde este irá retornar sua string em CAPS LOCK. Caso queira parar a execução, apenas digite **"exit"**. 

Vale a pena dizer que o proxy está rodando na porta 7000 e o server na 8080.
