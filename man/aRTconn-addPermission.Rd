\name{aRTconn-addPermission}
\alias{addPermission}
\alias{addPermission,aRTconn-method}
\title{Adds permission to a user}
\description{
	Function to grant some permissions in a DBMS for a user.
	The aRTconn user must have permissions for doing this operation (Only root can do that).
}
\usage{
\S4method{addPermission}{aRTconn}(object, user, pass, remote, host, privilege, db)
}
\arguments{
  \item{object}{Object of class \code{aRTconn}.}
  \item{user}{The username that will reveive the privileges.}
  \item{pass}{The password for acessing the DBMS. Empty string if none (the default). Note that the default Windows installation requires a password, otherwise it will print the error \code{Cant find any matching row in the user table}.}
  \item{remote}{A boolean value, representing whether the DBMS will enable this connection from external hosts. Note that the DBMS must have external connections enabled.}
  \item{host}{The external host to be acessed. If \code{remote} is true and \code{host} is empty string then the access is allowed from any host.}
  \item{privilege}{Defines DBMS privileges. It must be one of \code{"all"},  \code{"select"},  \code{"create"},  \code{"alter"} or  \code{"update"}. Execute \code{addPermission} repeatedly to combine two (or more) privileges.}
  \item{db}{The target database. If it is empty string the user will have permissions to access all databases.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	\code{aRTconn-class} shows the description of this class.
}
% vim: syntax=tex
