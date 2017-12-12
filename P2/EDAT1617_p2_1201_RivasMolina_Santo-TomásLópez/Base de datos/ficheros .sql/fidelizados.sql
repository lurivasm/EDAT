-- Table: public.fidelizados

-- DROP TABLE public.fidelizados;

CREATE TABLE public.fidelizados
(
  id_fidelizado integer NOT NULL,
  tarjeta text,
  gastos money,
  nickname text,
  fecha_expiracion date,
  fecha_alta timestamp without time zone,
  nombre text,
  CONSTRAINT fidelizados_pkey PRIMARY KEY (id_fidelizado)
)
WITH (
  OIDS=FALSE
);
ALTER TABLE public.fidelizados
  OWNER TO alumnodb;
