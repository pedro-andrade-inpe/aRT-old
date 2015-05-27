\name{aRTconn}

\docType{class}

\alias{aRTconn-class}
\alias{initialize,aRTconn-method}
\alias{aRTconn}
\alias{openConn}

\title{Class aRTconn}
\description{Class to store a virtual DBMS connection and manipulate databases.
The connection is virtual because each time a function of this class 
is called, it connects, does the stuff, and then disconnects.}

\section{Objects from the Class}{
Objects can be created using calls of the form \code{openConn(user, password, port, host)}.
}

\section{Arguments}{
  \describe{
	\item{user}{User of the DBMS. The default is the username logged in the system.}
	\item{password}{The password, default is \dQuote{}.}
    \item{dbms}{The Database Management System, It can be \dQuote{postgre} or \dQuote{mysql}, according to your installation.}
	\item{port}{Port of the connection. The default depends on the DBMS.}
	\item{host}{Host to connect, default is \dQuote{}, that means \dQuote{localhost}.}
	\item{name}{The connection's name. See below.}
  }
}

\section{Extends}{
Class \code{aRTcomponent}, directly.
}

\section{Methods}{
  \describe{
	\item{createDb:}{Creates a new database and returns a connection to it.}
	\item{deleteDb:}{Removes a database from the DBMS.}
	\item{openDb:}{Opens a database.}
    \item{showDbs:}{Lists the available databases.}
	\item{addPermission:}{Grants permissions on the DBMS to a given user.}
	\item{getPermissions:}{Lists the permissions for all users of a DBMS.}
	\item{dropUser:}{Removes permissions of a user in the DBMS.}
  }
}

\section{Connection names}{
aRT supports config files storing information about the user's connections.
Each user can have his/her own config file, called .aRTrc and located in the user's
home directory. An example of file is shown below:

 [root]\cr
 user=root\cr
 password=abc123\cr
 dbms=mysql\cr
 [pataxo]\cr
 host=pataxo.est.ufpr.br\cr
 user=pedro\cr
\cr

Each line starting with \dQuote{[} and ending with \dQuote{]}
is a name for a connection, which can be used in the \dQuote{name}
argument of \code{openConn()}.
The lines below a connection name describe the values for the parameters of the respective connection.
In this file we have two connections, one called \dQuote{root}, with an user, a password, and a dbms,
and the other called \dQuote{pataxo}, with a host and a user.
The arguments in the description are used as default values for the parameters of \code{openConn()}.
Whenever \code{openConn()} is called without any argument, it looks for a connection named
\dQuote{default} and use its values.

WARNING: If this file contains passwords or any other confidential information, 
set its permissions correctly (reading permission only to its owner).
}

\author{Marcos Aurelio Carrero, Pedro Ribeiro de Andrade}

\seealso{
  See \code{\link{aRT}} the package organization.
}

\keyword{classes}
% vim: syntax=tex
