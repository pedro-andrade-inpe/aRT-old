#require(multicore)
# http://cran.r-project.org/web/packages/argosfilter/argosfilter.pdf
# http://www.movable-type.co.uk/scripts/latlong.html
# distance(lat1, lat2, lon1, lon2)
# distances in km

calculateAngularDistancePoints = function(p1, p2)
{
	require(argosfilter)
	distance(p1[1], p2[1], p1[2], p2[2])
}

calculateEuclideanDistancePoints = function(p1, p2)
	sqrt((p1[1] - p2[1])^2 + (p1[2] - p2[2])^2)

calculateLineLength = function(line, distfunc, internal=1)
{
    qtde_points = dim(line[1]@lines[[1]]@Lines[[internal]]@coords)[1]
    coords = line[1]@lines[[1]]@Lines[[internal]]@coords

    d = 0
    for(k in 1:(qtde_points - 1))
    {
        d = d + distfunc(coords[k,], coords[k+1,])
    }
    return(d)
}

calculateLineLengthCoord = function(line, distfunc)
{
    qtde_points = dim(line)[1]
    coords = line

    d = 0
    for(k in 1:(qtde_points - 1))
    {
        d = d + distfunc(coords[k,], coords[k+1,])
    }
    return(d)
}

# distancia entre p0 e o segmento segA->segB
calculateDistancePointSegment = function(p, segA, segB, distfunc)
{
    p2 = c(segB[1] - segA[1], segB[2] - segA[2])
    something = p2[1]*p2[1] + p2[2]*p2[2]

    if(something == 0)
    {
    	#na verdade --> segA = segB
    	x = segA[1]
    	y = segA[2]
    }
    else
    {
        u = ((p[1] - segA[1]) * p2[1] + (p[2] - segA[2]) * p2[2]) / something

        if (u > 1)
            u = 1
        else if (u < 0)
            u = 0

        x = segA[1] + u * p2[1]
        y = segA[2] + u * p2[2]
    }

    return(distfunc(c(x, y), p))
}

calculatePointFromPointSegment = function(p, segA, segB, dist, distfunc)
{
    dpb = distfunc(p, segB)
    dpa = distfunc(p, segA)

	if(dpb <= dist) return(segB)
	if(dpa <= dist) return(segA)

    if(dpb > dpa) { dpb = dpa; other = segB; segB = segA; segA = other }

    difference = dpb^2 - dist^2
    dab = distfunc(segA, segB)

    if(dab != 0 && difference > 0)
    {
    	k = sqrt(dpb^2 - dist^2)
        ly = (k * (segA[2] - segB[2]))/dab
        lx = (k * (segA[1] - segB[1]))/dab
    }
    else
    {
		# it should never enter here
			print("ELSE A PROBLEM")
    	return(segB)
    }
	
    return(c(segB[1] + lx, segB[2] + ly))
}

firstPointOfLine = function(lines, internal=1) 
    lines@lines[[1]]@Lines[[internal]]@coords[1,]

lastPointOfLine = function(lines, internal=1)
    lines@lines[[1]]@Lines[[internal]]@coords[dim(lines@lines[[1]]@Lines[[internal]]@coords)[1],]

addEntryPoints = function(spatiallines, points)
{
    lines_ids = getID(spatiallines)

    quantity = dim(points)[1]
    for(i in 1:(dim(points)[1]))
    {
        .catSilent("Processing point", i, "out of", quantity, "\n")
        point = points[i,]@coords

        dd = 1e100
        pos = 0
        line = 0
        # looks for the closest point of any line to the destination 'points'
        for (j in 1:length(lines_ids))
        {
            qtde_points = dim(spatiallines@lines[[j]]@Lines[[1]]@coords)[1]
            coords = spatiallines@lines[[j]]@Lines[[1]]@coords

            for(k in 1:(qtde_points - 1))
            {
                d = calculateDistancePointSegment(point, coords[k,], coords[k+1,], calculateEuclideanDistancePoints)
                if (d < dd)
                {
                    dd = d
                    kk = k
                    jj = j
                }
            }
        }
        
        coords = spatiallines@lines[[jj]]@Lines[[1]]@coords

        .catSilent(paste("Ensuring minimum distance of ", dd, "km from the network to destination ", i, "\n", sep=""))

        # new point to be added to the network
        pto = calculatePointFromPointSegment(point, coords[kk,], coords[kk+1,], dd, calculateEuclideanDistancePoints)
        line1 = rbind(coords[1:kk,], pto)
        line2 = rbind(pto, coords[(kk+1):(dim(coords)[1]),])

        condition1 = (pto[1] == coords[kk,1] && pto[2] == coords[kk,2])
        if (!is.na(condition1) && condition1)
        {
            .catSilent("First point is equal, removing it from the line segment\n")
            line1 = coords[1:kk,]
        }
        
        condition2 = pto[1] == coords[kk+1,1] && pto[2] == coords[kk+1,2]
        if(!is.na(condition2) && condition2)
        {
            .catSilent("Second point is equal, removing it from the line segment\n")
            line2 = coords[(kk+1):(dim(coords)[1]),]
        }

        if(class(line1) != "numeric" && class(line2) != "numeric") # in the case of not splitting the starting or ending point
        {
            .catSilent("Adding the new line segment and updating the other\n")
            other_position = length(spatiallines@lines[[jj]]@Lines)
            l = Line(line2)
            spatiallines@lines[[jj]]@Lines[[other_position+1]] = l
            spatiallines@lines[[jj]]@Lines[[1]]@coords = line1
	    }
	    else
        {
            .catSilent("No need to add a line segment\n")
        }
    }
    return(spatiallines)
}

######################################################################################################
## calcula os pontos extremos de cada linha, colocando em uma lista com a contagem
## de repeticoes destes pontos

calculateNodes = function(spatiallines)
{
    point2char = function(point) paste(point[1],"..",point[2],sep="")

    xx = c()
    yy = c()
    netpoints = vector()

    ids = getID(spatiallines)
    len = length(ids)
    count = 1
    mcount = 1
    for (i in ids)
    {
        .catSilent("Generating nodes for line ", mcount, " out of ", len, " (id = ", i, ")\n", sep="")
        point = firstPointOfLine(spatiallines[i])
        name = point2char(point)
        if(is.na(netpoints[name]))
        {
            netpoints[name] = count
            xx[count] = point[1]
            yy[count] = point[2]
            count = count + 1
        }
        point = lastPointOfLine(spatiallines[i])
        name = point2char(point)
        if(is.na(netpoints[name]))
        {
            netpoints[name] = count
            xx[count] = point[1]
            yy[count] = point[2]
            count = count + 1
        }
        mcount = mcount + 1
    }

    pts = cbind(xx, yy)
    sp = SpatialPoints(pts)
    nodes = SpatialPointsDataFrame(sp, data=data.frame(ID=paste(1:length(xx))))
    return(list(nodes=nodes, netpoints=netpoints))
}

initNodesDistance = function(nodes, origins, weightfunc, distfunc = NULL)
{
	if(is.null(distfunc)) distfunc = calculateEuclideanDistancePoints
	
	nodes@data[,"distance"] = 1e+15
	nodes@data[,"target_id"] = ""

	quantity = dim(origins)[1]
	for(i in 1:(dim(origins)[1]))
	{
        .catSilent("Calculating out-of-network distance to", i, "out of", quantity, "\n")
        distance = 1e+15
        origin = origins[i,]@coords
        pos = 0

        for(j in 1:(dim(nodes)[1]))
        {
            point = nodes[j,]@coords
            d = distfunc(origin, point)
            d = weightfunc(d)
            if (d < distance)
            {
                distance = d
                pos = j
            }
        }
        nodes@data[pos,"distance"] = distance
        nodes@data[pos,"target_id"] = as.character(origins@data[i,"ID"])
    }
    return(nodes)
}

createConnectivityMatrix = function(slines, netpoints, weightf, distfunc = NULL)
{
	if(is.null(distfunc)) distfunc = calculateEuclideanDistancePoints

    point2char = function(point) paste(point[1],"..",point[2],sep="")

    ids = getID(slines)

    size = length(netpoints)
    m = matrix(1e+15, nrow = size, ncol = size)

    quantity = length(ids) 
    for (i in 1:length(ids))
    {
			.catSilent("Adding line", i, "out of", quantity, "\n")

			mylines = slines[i]
			for(j in 1:length(mylines@lines[[1]]@Lines))
			{
				point1 = firstPointOfLine(mylines, j)
				name1 = point2char(point1)

				llength = calculateLineLength(mylines, distfunc, j)
				dist = weightf(llength, ids[i])
				point2 = lastPointOfLine(mylines, j)
				name2 = point2char(point2)
	
				# matriz simetrica
				m[netpoints[name1][[1]], netpoints[name2][[1]]] = dist
				m[netpoints[name2][[1]], netpoints[name1][[1]]] = dist
			}
    }
    return(m)
}

updateDistances = function(nodes, connect_matrix)
{
    reduce = TRUE
    count = 0
    while(reduce)
    {
        count = count + 1
        .catSilent("Updating", count, "out of", dim(nodes)[1], "(maximum)\n")

        reduce = FALSE
        for(i in 1:(dim(nodes)[1]))
        {
            dist = nodes@data[i,"distance"]
            newdists = connect_matrix[i,] + nodes@data[,"distance"]
            j = which.min(newdists)
            newdist = newdists[j]

            if(dist > newdist)
            {
                nodes@data[i,"distance"] = newdist
                nodes@data[i,"target_id"] = nodes@data[j,"target_id"]
                reduce = TRUE
            }
        }
    }
    return(nodes)
}

getNeighborsOpenNetworkFunction = function(points, network, weighttoenterfunc = NULL, distfunc = NULL)
{
	if (is.null(weighttoenterfunc)) weighttoenterfunc = function(x) x
	if (is.null(distfunc)) distfunc = calculateEuclideanDistancePoints

	spatiallines = network$network;centroide_lotes = points
	mynodes = network$nodes;netpoints = network$references
	origin_ids  = getID(centroide_lotes);lines_ids = getID(spatiallines)
	return (function(id)
	{
		idpos = which(origin_ids == id)
		point = centroide_lotes[idpos,]@coords
		dd = 1e+15
		pos = 0
		line = 0
		for (j in 1:length(lines_ids))
		{
			qtde_points = dim(spatiallines@lines[[j]]@Lines[[1]]@coords)[1]
			coords = spatiallines@lines[[j]]@Lines[[1]]@coords
			for(k in 1:(qtde_points - 1))
			{
				d = calculateDistancePointSegment(point, coords[k,], coords[k+1,], distfunc)
				if (d < dd)
				{
					dd = d;  kk = k; jj = j
				}
			}
		}
		coords = spatiallines@lines[[jj]]@Lines[[1]]@coords
		pto = calculatePointFromPointSegment(point, coords[kk,], coords[kk+1,], dd, distfunc)
		line1 = rbind(coords[1:kk,], pto)
		line2 = rbind(pto, coords[(kk+1):(dim(coords)[1]),])
		len1 = calculateLineLengthCoord(line1, distfunc)
		len2 = calculateLineLengthCoord(line2, distfunc)

		point2char = function(point) paste(point[1],"..",point[2],sep = "")
		ch1 = point2char(coords[1,])

		dist1 = mynodes@data[netpoints[ch1], "distance"]
		ch2 = point2char(coords[dim(coords)[1],])

		dist2 = mynodes@data[netpoints[ch2], "distance"]
		d_btw_points = network$cmatrix[netpoints[ch1],netpoints[ch2]]
		mlen1 = (len1 / (len1 + len2)) * d_btw_points
		mlen2 = (len2 / (len1 + len2)) * d_btw_points

		dist1 = weighttoenterfunc(dist1)
		dist2 = weighttoenterfunc(dist2)

		if (dist1 + mlen1 < dist2 + mlen2)
		{
			com1 = mynodes@data[netpoints[ch1], "target_id"]
			return(list(ids = com1, distance = dist1 + mlen1 + dd))
		}
		else
		{
			com2 = mynodes@data[netpoints[ch2], "target_id"]
			return(list(ids = com2, distance = dist2 + mlen2 + dd))
		}
	})
}

getNeighborsDistanceFunction = function(origin_points, destination_points, distfunc)
{
	if(is.null(distfunc)) distfunc = calculateEuclideanDistancePoints
	origin_ids  = getID(origin_points)
	destination_ids = getID(destination_points)

	return (function(id)
	{
		pos = which(origin_ids == id)
		point1 = origin_points[pos,]@coords
		dd = 1e+15
		pos = 0
		for (j in 1:length(destination_ids))
		{
			point2 = destination_points[j,]@coords
	
			d = distfunc(point1, point2)
	
			if (d < dd)	
			{
				dd = d
				pos = j
			}
		}

		neighbor_id = as.character(destination_points@data[pos,"ID"])
		return (list(ids = neighbor_id, distance = dd))
	})
}

getNeighborsMaxDistanceFunction = function(points, max_dist, dist_function = NULL)
{
	if(is.null(dist_function)) dist_function = calculateEuclideanDistancePoints
	ids = getID(points)
	
	return (function(id)
	{
		pos = which(ids == id)
		point1 = points[pos,]@coords
		qtde = 0
		result_ids = vector()
		result_distances = vector()

		for (j in 1:length(ids))
		{
			if(j != pos)
			{
				point2 = points[j,]@coords

				d = dist_function(point1, point2)
	
				if (d < max_dist)	
				{
					qtde = qtde + 1
					result_ids = c(result_ids, as.character(points@data[j,"ID"])) ##       ids[j])
					result_distances = c(result_distances, d)
				}
			}
		}

		return (list(ids = result_ids, distance = result_distances))
	})
}

createOpenNetwork = function(destinations, network, weightwithinnetfunc, weighttoenterfunc = NULL, distfunc = NULL)
{
	if(is.null(weighttoenterfunc)) weighttoenterfunc = function(x) x
	if(is.null(distfunc)) distfunc = calculateEuclideanDistancePoints

	.catSilent("Step 1/5 (adding points)\n")
  # add points closer to the destination points in order to guarantee
  # a minimum distance from the network to the destination
  # it break the closest line to each destination point whenever necessary
	full_network = addEntryPoints(network, destinations)

	.catSilent("Step 2/5 (generating network)\n")
	# generate the nodes from the network
	# only such nodes will store the costs
	# if the origin point enter in the network in the middle
	# of a line then it will calculate the closest of the two
	# points and add a proportional distance to the closst point
	x = calculateNodes(full_network)
	network_nodes = x$nodes
	network_references = x$netpoints

	.catSilent("Step 3/5 (starting distances)\n")
	# start the costs, storing only the nodes of the network that are
	# closest to the destination points
	full_mynodes = initNodesDistance(network_nodes, destinations, weighttoenterfunc, distfunc)

	.catSilent("Step 4/5 (creating connectivity matrix)\n")
	# fill all the connectivity matrix based on the initial
	# costs above plus the costs on the network
	connectivity_matrix = createConnectivityMatrix(full_network, network_references, weightwithinnetfunc, distfunc)

	.catSilent("Step 5/5 (updating distances)\n")
	# once the matrix is defined, compute the entrance point to the
	# matrix and then use the cost already defined by the matrix
	# as cost to the destination
	mynodes_initial = updateDistances(full_mynodes, connectivity_matrix)

	return(list(network=full_network, nodes = mynodes_initial, references = network_references, cmatrix = connectivity_matrix))
}

connectToBiggerIntersectionArea = function(spcells, lpolygons)
{
	artcells = as.aRTgeometry(spcells)
	id_cells = getID(spcells)
	table_cells = data.frame(id=id_cells, area=0, father="", stringsAsFactors=FALSE)
	rownames(table_cells) = id_cells

	q = openQuerier(lpolygons)
	for(i in 1:(summary(q)$elements))
	{
		d = getData(q, 1, as.sp=FALSE)
		father = getID(getGeometry(d))
		clip = getClip(artcells, d)
		if(!is.null(clip))
		{
			ids = getID(clip)
			for(j in 1:length(ids))
			{
				idj = ids[j]
				area = clip@polygons[[j]]@area
				if(table_cells[idj,"area"] < area)
				{
					table_cells[idj, "area"] = area
					table_cells[idj, "father"] = father
				}
			}
		}
	}
	return(table_cells)
}


setMethod("createGPM", "aRTlayer", function(object, get_neighbors)
{
	ids = getID(object)

	result = list()

	cont = 1
	for(i in ids)
	{
		.catSilent("Processing element ", i, "\n")
		rel = get_neighbors(i)
		result[[i]] = rel
		cont = cont + 1
	}
	return (result)
})

## Raian: Connect a cell intersection lines
getNeighborsIntersectionLines = function(lcells, lalines)
{
 	return(function(id)
 	{
		  parents = getRelation(lalines, c("within", "crosses"), lcells, id)
		  return(list(ids=parents))
 	})

}

## Raian: Connect a cell with contained points
getNeighborsContainedPoints = function(lcells, lapoints)
{
 	return(function(id)
 	{
		parents = getRelation(lapoints, "contains", lcells, id)
		return(list(ids=parents))
 	})
}

## Raian: Connect a line with intersection polygons
connectLineToIntersectionPolygons = function(lalines, lpolygons)
{
 	return(function(id)
 	{
		parents = getRelation(lpolygons, c("within", "crosses"), lalines, id)
		return(list(ids=parents))
 	})
}

## file extensions supported: .gpm, .gal and .gwt
saveGPM = function(gpm, filename, layer1, layer2=NULL, key="object_id_", attrib=NULL)
{
	tkns = unlist(strsplit(filename, "\\."))
	lenTkns = length(tkns)
	extension = tkns[lenTkns]

	.catSilent("Saving the GPM in a \".", extension, "\" file ...", sep="")

	if(identical(extension, "gpm"))
	{
	    ids = names(gpm)
	    qtde = length(ids)

	    zz <- file(filename, "w")
	    
	    if(is.null(layer2))
		    layer2 = layer1;

	    attribs = names(gpm[[ids[1]]])
	    countAttribs = 0
	    numAttribs = length(attribs) - 1

	    cat(c(numAttribs, "\t", layer1, "\t", layer2), "\t", sep="", file=zz)
	    
	    for(i in attribs)
	    {
		    if(identical(i, "ids") == FALSE)
			 cat(i, "\t", sep="", file=zz)
	    }
	    cat("\n", file=zz);

	    for (i in ids)
	    {
		    cat(i, "\t", sep="", file=zz)                   		 # ID
		    
		    if(length(gpm[[i]]$ids[1]) != 0)
		    {
			  if(gpm[[i]]$ids[1] != "") 		# if has no neighbors, print 0
			  {
				  cat(length(gpm[[i]]$ids), "\n", sep="", file=zz) # size
				  
				  idNeighs = gpm[[i]]$ids;
				  count = 1;
				  
				  for(j in idNeighs)
				  {
					  cat(j, "\t", sep="", file=zz)   # neighbors
					  if(length(attribs) > 1)
					  {
						  for(k in attribs)
						  {
						      if(identical(k, "ids") == FALSE)
							  cat(gpm[[i]][[k]][count], "\t", sep="", file=zz);	#weights
						  }
					  }
					  count = count + 1;
				  }
				  cat("\n", file=zz);
			  }
			  else
				  cat(0, "\n", sep="", file=zz)
		    }
		    else
			  cat(0, "\n", sep="", file=zz)
	    }
	    close(zz)
	}
	else if(identical(extension, "gal") || identical(extension, "GAL"))
	{
	    if(is.null(layer2))
	    {
		    ids = names(gpm)
		    qtde = length(ids)

		    zz <- file(filename, "w")

		    cat(c(0, qtde, layer1, key), "\n", sep="\t", file=zz) # Size, Layer and key variable

		    for (i in ids)
		    {
			    cat(i, "\t", file=zz)                   		 # ID
			    if(gpm[[i]]$ids[1] != "")
			    {
				    cat(length(gpm[[i]]$ids), sep="\n", file=zz) # size
				    cat(gpm[[i]]$ids, "\n", sep="\t", file=zz)   # neighbors
			    }
			    else
				    cat(0, sep="\n", file=zz)
		    }
		    close(zz)
	     }
			else
			{
				.catSilent("no\n", sep="")
				stop("GAL format file does not support neighborhoods between two layers. Use the GPM format.")
			}
	}
	else if(identical(extension, "gwt") || identical(extension, "GWT"))
	{
	    if(is.null(layer2))
	    {
		    ids = names(gpm)
		    qtde = length(ids)

		    zz <- file(filename, "w")

		    cat(c(0, qtde, layer1, key), "\n", sep="\t", file=zz)              # size

		    for (i in ids)
		    {
			    idNeighs = gpm[[i]]$ids
			    count = 1
			    for(j in idNeighs)
			    {
				    cat(i, "\t", sep="", file=zz)                    # ID cell
				    cat(j, "\t", sep="", file=zz) 					 # ID neighbor
				    if(!is.null(attrib))
					    cat(gpm[[i]][[attrib]][count], file=zz) # distance 
												  # 
				    count = count + 1
				    cat("\n", file=zz)
			    }
		    }
		    close(zz)
	    }
	    else
			{
				.catSilent("no\n", sep="")
		    stop("GWT format file does not support neighborhoods between two layers. Use the GPM format.")
			}
	}
	else
	{
			.catSilent("no\n", sep="")
	    stop("Extension not supported!!! The extensions supported are: \".gpm\", \".gal\"/\".GAL\" and \".gwt\"/\".GWT\"");
	}

	.catSilent("yes\n", sep="")
}

