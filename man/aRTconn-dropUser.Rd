\name{aRTconn-dropUser}
\alias{dropUser}
\alias{dropUser,aRTconn-method}
\title{Drops an user}
\description{
	Function to remove permissions of an user in the database.
	The aRTconn user must have permissions for doing this operation (user root has this permission).
}
\usage{
\S4method{dropUser}{aRTconn}(object, user, remote, host)
}
\arguments{
  \item{object}{Object of class \code{aRTconn}.}
  \item{user}{The user that will be removed.}
  \item{remote}{A boolean value meaning whether the DBMS will remove the access from external hosts.}
  \item{host}{The host to be removed for access. If \code{remote} is true and  \code{host} is empty string then the access is removed from any host.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	\code{aRTconn-class} shows the description of this class.
}
% vim: syntax=tex
