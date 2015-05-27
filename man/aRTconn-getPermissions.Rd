\name{aRTconn-getPermissions}
\alias{getPermissions}
\alias{getPermissions,aRTconn-method}
\title{Gets the permissions in a DBMS}
\description{
	Function to see the permissions in a DBMS.
}
\usage{
\S4method{getPermissions}{aRTconn}(object, user, global)
}
\arguments{
  \item{object}{Object of class \code{aRTconn}.}
  \item{user}{The user which the permissions will be shown. If none, shows all the permissions of all users.}
  \item{global}{A boolean value. If true, it shows the permissions on the DBMS, which means that the user has permissions on every database. Otherwise, this function returns the specific permissions on each database.}
}

\author{Pedro Ribeiro de Andrade}
\keyword{database}
\seealso{
	\code{aRTconn-class} shows the description of this class.
}
% vim: syntax=tex
