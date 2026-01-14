CREATE SCHEMA IF NOT EXISTS clinica;
CREATE DOMAIN clinica.tipo_cpf AS BIGINT;
CREATE TABLE IF NOT EXISTS clinica.Usuario (
    cpf clinica.tipo_cpf,
    Cargo VARCHAR(10),
    Nome VARCHAR(60) NOT NULL,
    
    CONSTRAINT pk_usuario PRIMARY KEY (cpf)
);

CREATE TABLE IF NOT EXISTS clinica.Enfermidade (
    Nome VARCHAR(150) NOT NULL UNIQUE,
    Codigo_CID VARCHAR(6) NOT NULL,
    
    CONSTRAINT pk_Enfermidade PRIMARY KEY (Codigo_CID)
);

CREATE TABLE IF NOT EXISTS clinica.Medico (
    id_medico VARCHAR(10) NOT NULL,
    cpf  clinica.tipo_cpf UNIQUE,
    
  	CONSTRAINT pk_medico PRIMARY KEY (id_medico),
    CONSTRAINT fk_id_usuario FOREIGN KEY (cpf) REFERENCES clinica.Usuario (cpf) ON DELETE CASCADE ON UPDATE CASCADE
    
    
);

CREATE TABLE IF NOT EXISTS clinica.Enfermeiro (
    id_Enfermeiro VARCHAR(10) NOT NULL,
    cpf  clinica.tipo_cpf UNIQUE,
    
    CONSTRAINT fk_id_usuario FOREIGN KEY (cpf) REFERENCES clinica.Usuario (cpf) ON DELETE CASCADE ON UPDATE CASCADE,
    
    CONSTRAINT pk_id_Enfermeiro PRIMARY KEY (id_Enfermeiro),
    
);

CREATE TABLE IF NOT EXISTS clinica.Secretario (
    id_Secret VARCHAR(10) NOT NULL,
    cpf  clinica.tipo_cpf UNIQUE,
    
    CONSTRAINT fk_usuario FOREIGN KEY (cpf) REFERENCES clinica.Usuario (cpf) ON DELETE CASCADE ON UPDATE CASCADE,
    
    CONSTRAINT pk_id_Secret PRIMARY KEY (id_Secret)
  
);

CREATE TABLE IF NOT EXISTS clinica.Afericao (
    Nome VARCHAR(50) NOT NULL,
    Resultado VARCHAR(100) NULL,
    id_afericao VARCHAR(4) NOT NULL,
    
    CONSTRAINT id_afericao PRIMARY KEY (id_afericao)
);

CREATE TABLE IF NOT EXISTS clinica.Medicamento (
    Concentracao VARCHAR(5) NOT NULL,
    Subst VARCHAR(60) NOT NULL,
    id_medicamento VARCHAR(10) NOT NULL,
    
    CONSTRAINT id_medicamento PRIMARY KEY (id_medicamento),
    CONSTRAINT uq_medicamento UNIQUE (Subst, Concentracao)
);

CREATE TABLE IF NOT EXISTS clinica.Paciente (
    id_paciente VARCHAR(10) NOT NULL,
    idade INT NOT NULL,
    nome VARCHAR(60) NOT NULL,
    data_nascimento DATE NOT NULL,
    
    CONSTRAINT id_paciente PRIMARY KEY (id_paciente)
);

CREATE TABLE IF NOT EXISTS clinica.Diagnostico (
    Nome VARCHAR(150) NOT NULL UNIQUE,
    Codigo_CID VARCHAR(6) NOT NULL,
    DiagnostID VARCHAR(10) NOT NULL,
    
    CONSTRAINT DiagnostID PRIMARY KEY (DiagnostID),
    
    CONSTRAINT fk_diagnostico_enfermidade FOREIGN KEY (Codigo_CID) REFERENCES clinica.Enfermidade (Codigo_CID) ON DELETE CASCADE ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS clinica.Exame (
    Tipo VARCHAR(50) NOT NULL,
    data_exame DATE NOT NULL,
    Conclusao VARCHAR(100),
    Resultados VARCHAR(100) NOT NULL,
    id_exame VARCHAR(10) NOT NULL,
    
    CONSTRAINT id_exame PRIMARY KEY (id_exame)
);

CREATE TABLE IF NOT EXISTS clinica.Formulario (
    id_Formulario VARCHAR(10) NOT NULL,
    idExames VARCHAR(10),
    Codigo_CID VARCHAR(6),
    id_Afericao VARCHAR(10),
    
    CONSTRAINT id_Formulario PRIMARY KEY (id_Formulario),
    
    CONSTRAINT fk_id_Afericao
    FOREIGN KEY (id_Afericao)
    REFERENCES clinica.Afericao (id_afericao)
    ON DELETE SET NULL
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_idExames
    FOREIGN KEY (idExames)
    REFERENCES clinica.Exame (id_exame)
    ON DELETE SET NULL
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_enfermidade_formulario
    FOREIGN KEY (Codigo_CID)
    REFERENCES clinica.Enfermidade (Codigo_CID)
    ON DELETE SET NULL
    ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS clinica.Prontuario (
    id_Prontuario VARCHAR(10) NOT NULL,
    id_Diagnostico VARCHAR(10) NOT NULL,
    id_Formulario VARCHAR(10) NOT NULL,
    
    CONSTRAINT id_Prontuario PRIMARY KEY (id_Prontuario),
    
    CONSTRAINT fk_Diagnostico
    FOREIGN KEY (id_Diagnostico)
    REFERENCES clinica.Diagnostico (DiagnostID)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_Forumulario
    FOREIGN KEY (id_Formulario)
    REFERENCES clinica.Formulario (id_Formulario)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS clinica.Solicitacao (
    id_Solicitacao VARCHAR(10) NOT NULL,
    Nome VARCHAR(45) NOT NULL,
    Especialidade VARCHAR(45) NULL,
    Tipo_Atendimento VARCHAR(45) NOT NULL,
    
    CONSTRAINT id_Solicitacao PRIMARY KEY (id_Solicitacao)
);

CREATE TABLE IF NOT EXISTS clinica.Procedimentos (
    id_Procedimentos VARCHAR(10) NOT NULL,
    id_medicamento VARCHAR(10) NOT NULL,
    
    CONSTRAINT id_Procedimentos PRIMARY KEY (id_Procedimentos),
    
    CONSTRAINT fk_id_Medicamento
    FOREIGN KEY (id_medicamento)
    REFERENCES clinica.Medicamento (id_medicamento)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS clinica.Consulta (
    Data_Consulta DATE NOT NULL,
    id_consulta VARCHAR(10) NOT NULL,
    id_equipe VARCHAR(10) NOT NULL,
    id_Prontuario VARCHAR(10) NOT NULL,
    id_Procedimentos VARCHAR(10) NULL,
    id_Solicitacao VARCHAR(10) NULL, 
    id_Paciente VARCHAR(10) NULL,
    
    CONSTRAINT id_consulta PRIMARY KEY (id_consulta),
    CONSTRAINT uq_consulta UNIQUE (Data_Consulta, id_consulta),
    CONSTRAINT fk_id_Medico_consulta
    FOREIGN KEY (id_medico)
    REFERENCES clinica.Medico (id_Medico)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_id_Paciente
    FOREIGN KEY (id_Paciente)
    REFERENCES clinica.Paciente (id_Paciente)
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_id_Prontuario
    FOREIGN KEY (id_Prontuario)
    REFERENCES clinica.Prontuario (id_Prontuario)
    ON DELETE NO ACTION
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_id_Solicitacao
    FOREIGN KEY (id_Solicitacao)
    REFERENCES clinica.Solicitacao (id_Solicitacao)
    ON DELETE SET NULL
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_id_Procedimentos
    FOREIGN KEY (id_Procedimentos)
    REFERENCES clinica.Procedimentos (id_Procedimentos)
    ON DELETE SET NULL
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_id_Equipe
    FOREIGN KEY (id_equipe)
    REFERENCES clinica.Equipe (id_equipe)  
    ON DELETE NO ACTION
    ON UPDATE CASCADE
);

CREATE TABLE IF NOT EXISTS clinica.Receituario (
    id_Receituario VARCHAR(10) NOT NULL,
    id_consulta VARCHAR(10) NOT NULL,
    id_medicamento VARCHAR(10) NOT NULL,
  	id_instrucao VARCHAR(10) NOT NULL,
    CONSTRAINT id_Receituario PRIMARY KEY (id_Receituario),


    CONSTRAINT fk_id_medicamento_receituario
    FOREIGN KEY (id_medicamento)
    REFERENCES clinica.Medicamento (id_medicamento)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
  
  	CONSTRAINT fk_id_instrucao
    FOREIGN KEY (id_instrucao)
    REFERENCES clinica.Instrucao (id_instrucao)
    ON DELETE NO ACTION
    ON UPDATE NO ACTION
);

CREATE TABLE IF NOT EXISTS clinica.Instrucao (
    modo_de_uso VARCHAR(50) NOT NULL,
    qtd DECIMAL(2) NOT NULL,
    Frequencia VARCHAR(45) NOT NULL,
    id_receituario VARCHAR(10) NOT NULL,
    id_instrucao VARCHAR(10) NOT NULL,
    
    CONSTRAINT pk_instrucao PRIMARY KEY (id_instrucao),
    
);

CREATE TABLE IF NOT EXISTS clinica.Equipe (
    id_equipe VARCHAR(10) NOT NULL,
    zona VARCHAR(10) NOT NULL,
    id_medico VARCHAR(10) NOT NULL,
    CONSTRAINT fk_id_Medico_consulta
    FOREIGN KEY (id_medico)
    REFERENCES clinica.Medico (id_Medico)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
    CONSTRAINT id_equipe PRIMARY KEY (id_equipe),
  	
);

CREATE TABLE IF NOT EXISTS clinica.EnfermeiroEquipe (
    id_Enfermeiro VARCHAR(10) NOT NULL,
    id_equipe VARCHAR(10) NOT NULL,
    
    CONSTRAINT pk_enf_eq PRIMARY KEY (id_Enfermeiro, id_equipe),
    
    CONSTRAINT fk_id_Enfermeiro_equipe
    FOREIGN KEY (id_Enfermeiro)
    REFERENCES clinica.Enfermeiro (id_Enfermeiro)
    ON DELETE CASCADE
    ON UPDATE CASCADE,
    
    CONSTRAINT fk_id_equipe
    FOREIGN KEY (id_equipe)
    REFERENCES clinica.Equipe (id_equipe)
    ON DELETE CASCADE
    ON UPDATE CASCADE
);

