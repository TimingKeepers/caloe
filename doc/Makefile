 # ****************************************************************************** 
 # @file Makefile
 #  @brief CALoE Library Makefile. It builds CALoE library doc.
 #
 #  Copyright (C) 2013
 #
 #  CALoE library makefile.
 #
 #  @author Miguel Jimenez Lopez <klyone@ugr.es>
 #
 #  @bug none!
 #
 # ******************************************************************************
 #  This library is free software; you can redistribute it and/or
 #  modify it under the terms of the GNU Lesser General Public
 #  License as published by the Free Software Foundation; either
 #  version 3 of the License, or (at your option) any later version.
 #
 #  This library is distributed in the hope that it will be useful,
 #  but WITHOUT ANY WARRANTY; without even the implied warranty of
 #  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 #  Lesser General Public License for more details.
 #  
 #  You should have received a copy of the GNU Lesser General Public
 #  License along with this library. If not, see <http//www.gnu.org/licenses/>.
 # ******************************************************************************

report:
	@echo "doc: Compiling doc (report)..."
	@pdflatex caloe.tex
	@bibtex caloe.aux
	@pdflatex caloe.tex
	@pdflatex caloe.tex

clean: 
	@echo "doc: Cleanup"
	@-rm *~ *.bbl *.blg *.log *.lof *.toc *.aux *.pdf
