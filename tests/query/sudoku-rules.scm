
;
; Sudoku puzzle rules
;

; Definition of a number
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "one"))
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "two"))
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "three"))
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "four"))
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "five"))
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "six"))
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "seven"))
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "eight"))
(EvaluationLink (PredicateNode "IsNumber") (ConceptNode "nine"))

; The set of numbers that can be chosen from
(SetLink
	(ConceptNode "one")
	(ConceptNode "two")
	(ConceptNode "three")
	(ConceptNode "four")
	(ConceptNode "five")
	(ConceptNode "six")
	(ConceptNode "seven")
	(ConceptNode "eight")
	(ConceptNode "nine")
)

; The contents of the cells must be numbers!
(define (cells_are_numbers)
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_11"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_12"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_13"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_14"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_15"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_16"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_17"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_18"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_19"))

	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_21"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_22"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_23"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_24"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_25"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_26"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_27"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_28"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_29"))

	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_31"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_32"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_33"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_34"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_35"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_36"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_37"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_38"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_39"))

	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_41"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_42"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_43"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_44"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_45"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_46"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_47"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_48"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_49"))

	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_51"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_52"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_53"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_54"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_55"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_56"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_57"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_58"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_59"))

	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_61"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_62"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_63"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_64"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_65"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_66"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_67"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_68"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_69"))

	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_71"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_72"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_73"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_74"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_75"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_76"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_77"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_78"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_79"))

	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_81"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_82"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_83"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_84"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_85"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_86"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_87"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_88"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_89"))

	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_91"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_92"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_93"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_94"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_95"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_96"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_97"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_98"))
	(EvaluationLink (PredicateNode "IsNumber") (VariableNode "$cell_99"))
)

; twenty-seven solution constraints
; First, nine row constraints
(define row1
	(SetLink
		(VariableNode "$cell_11")
		(VariableNode "$cell_12")
		(VariableNode "$cell_13")
		(VariableNode "$cell_14")
		(VariableNode "$cell_15")
		(VariableNode "$cell_16")
		(VariableNode "$cell_17")
		(VariableNode "$cell_18")
		(VariableNode "$cell_19")
	)
)
(define row2
	(SetLink
		(VariableNode "$cell_21")
		(VariableNode "$cell_22")
		(VariableNode "$cell_23")
		(VariableNode "$cell_24")
		(VariableNode "$cell_25")
		(VariableNode "$cell_26")
		(VariableNode "$cell_27")
		(VariableNode "$cell_28")
		(VariableNode "$cell_29")
	)
)
(define row3
	(SetLink
		(VariableNode "$cell_31")
		(VariableNode "$cell_32")
		(VariableNode "$cell_33")
		(VariableNode "$cell_34")
		(VariableNode "$cell_35")
		(VariableNode "$cell_36")
		(VariableNode "$cell_37")
		(VariableNode "$cell_38")
		(VariableNode "$cell_39")
	)
)
(define row4
	(SetLink
		(VariableNode "$cell_41")
		(VariableNode "$cell_42")
		(VariableNode "$cell_43")
		(VariableNode "$cell_44")
		(VariableNode "$cell_45")
		(VariableNode "$cell_46")
		(VariableNode "$cell_47")
		(VariableNode "$cell_48")
		(VariableNode "$cell_49")
	)
)
(define row5
	(SetLink
		(VariableNode "$cell_51")
		(VariableNode "$cell_52")
		(VariableNode "$cell_53")
		(VariableNode "$cell_54")
		(VariableNode "$cell_55")
		(VariableNode "$cell_56")
		(VariableNode "$cell_57")
		(VariableNode "$cell_58")
		(VariableNode "$cell_59")
	)
)
(define row6
	(SetLink
		(VariableNode "$cell_61")
		(VariableNode "$cell_62")
		(VariableNode "$cell_63")
		(VariableNode "$cell_64")
		(VariableNode "$cell_65")
		(VariableNode "$cell_66")
		(VariableNode "$cell_67")
		(VariableNode "$cell_68")
		(VariableNode "$cell_69")
	)
)
(define row7
	(SetLink
		(VariableNode "$cell_71")
		(VariableNode "$cell_72")
		(VariableNode "$cell_73")
		(VariableNode "$cell_74")
		(VariableNode "$cell_75")
		(VariableNode "$cell_76")
		(VariableNode "$cell_77")
		(VariableNode "$cell_78")
		(VariableNode "$cell_79")
	)
)
(define row8
	(SetLink
		(VariableNode "$cell_81")
		(VariableNode "$cell_82")
		(VariableNode "$cell_83")
		(VariableNode "$cell_84")
		(VariableNode "$cell_85")
		(VariableNode "$cell_86")
		(VariableNode "$cell_87")
		(VariableNode "$cell_88")
		(VariableNode "$cell_89")
	)
)
(define row9
	(SetLink
		(VariableNode "$cell_91")
		(VariableNode "$cell_92")
		(VariableNode "$cell_93")
		(VariableNode "$cell_94")
		(VariableNode "$cell_95")
		(VariableNode "$cell_96")
		(VariableNode "$cell_97")
		(VariableNode "$cell_98")
		(VariableNode "$cell_99")
	)
)

;; Next, nine column constraints
(define col1
	(SetLink
		(VariableNode "$cell_11")
		(VariableNode "$cell_21")
		(VariableNode "$cell_31")
		(VariableNode "$cell_41")
		(VariableNode "$cell_51")
		(VariableNode "$cell_61")
		(VariableNode "$cell_71")
		(VariableNode "$cell_81")
		(VariableNode "$cell_91")
	)
)
(define col2
	(SetLink
		(VariableNode "$cell_12")
		(VariableNode "$cell_22")
		(VariableNode "$cell_32")
		(VariableNode "$cell_42")
		(VariableNode "$cell_52")
		(VariableNode "$cell_62")
		(VariableNode "$cell_72")
		(VariableNode "$cell_82")
		(VariableNode "$cell_92")
	)
)
(define col3
	(SetLink
		(VariableNode "$cell_13")
		(VariableNode "$cell_23")
		(VariableNode "$cell_33")
		(VariableNode "$cell_43")
		(VariableNode "$cell_53")
		(VariableNode "$cell_63")
		(VariableNode "$cell_73")
		(VariableNode "$cell_83")
		(VariableNode "$cell_93")
	)
)
(define col4
	(SetLink
		(VariableNode "$cell_14")
		(VariableNode "$cell_24")
		(VariableNode "$cell_34")
		(VariableNode "$cell_44")
		(VariableNode "$cell_54")
		(VariableNode "$cell_64")
		(VariableNode "$cell_74")
		(VariableNode "$cell_84")
		(VariableNode "$cell_94")
	)
)
(define col5
	(SetLink
		(VariableNode "$cell_15")
		(VariableNode "$cell_25")
		(VariableNode "$cell_35")
		(VariableNode "$cell_45")
		(VariableNode "$cell_55")
		(VariableNode "$cell_65")
		(VariableNode "$cell_75")
		(VariableNode "$cell_85")
		(VariableNode "$cell_95")
	)
)
(define col6
	(SetLink
		(VariableNode "$cell_16")
		(VariableNode "$cell_26")
		(VariableNode "$cell_36")
		(VariableNode "$cell_46")
		(VariableNode "$cell_56")
		(VariableNode "$cell_66")
		(VariableNode "$cell_76")
		(VariableNode "$cell_86")
		(VariableNode "$cell_96")
	)
)
(define col7
	(SetLink
		(VariableNode "$cell_17")
		(VariableNode "$cell_27")
		(VariableNode "$cell_37")
		(VariableNode "$cell_47")
		(VariableNode "$cell_57")
		(VariableNode "$cell_67")
		(VariableNode "$cell_77")
		(VariableNode "$cell_87")
		(VariableNode "$cell_97")
	)
)
(define col8
	(SetLink
		(VariableNode "$cell_18")
		(VariableNode "$cell_28")
		(VariableNode "$cell_38")
		(VariableNode "$cell_48")
		(VariableNode "$cell_58")
		(VariableNode "$cell_68")
		(VariableNode "$cell_78")
		(VariableNode "$cell_88")
		(VariableNode "$cell_98")
	)
)
(define col9
	(SetLink
		(VariableNode "$cell_19")
		(VariableNode "$cell_29")
		(VariableNode "$cell_39")
		(VariableNode "$cell_49")
		(VariableNode "$cell_59")
		(VariableNode "$cell_69")
		(VariableNode "$cell_79")
		(VariableNode "$cell_89")
		(VariableNode "$cell_99")
	)
)

;; And finally, 9 box constraints
(define box11
	(SetLink
		(VariableNode "$cell_11")
		(VariableNode "$cell_12")
		(VariableNode "$cell_13")
		(VariableNode "$cell_21")
		(VariableNode "$cell_22")
		(VariableNode "$cell_23")
		(VariableNode "$cell_31")
		(VariableNode "$cell_32")
		(VariableNode "$cell_33")
	)
)

(define box14
	(SetLink
		(VariableNode "$cell_14")
		(VariableNode "$cell_15")
		(VariableNode "$cell_16")
		(VariableNode "$cell_24")
		(VariableNode "$cell_25")
		(VariableNode "$cell_26")
		(VariableNode "$cell_34")
		(VariableNode "$cell_35")
		(VariableNode "$cell_36")
	)
)

(define box17
	(SetLink
		(VariableNode "$cell_17")
		(VariableNode "$cell_18")
		(VariableNode "$cell_19")
		(VariableNode "$cell_27")
		(VariableNode "$cell_28")
		(VariableNode "$cell_29")
		(VariableNode "$cell_37")
		(VariableNode "$cell_38")
		(VariableNode "$cell_39")
	)
)

(define box41
	(SetLink
		(VariableNode "$cell_41")
		(VariableNode "$cell_42")
		(VariableNode "$cell_43")
		(VariableNode "$cell_51")
		(VariableNode "$cell_52")
		(VariableNode "$cell_53")
		(VariableNode "$cell_61")
		(VariableNode "$cell_62")
		(VariableNode "$cell_63")
	)
)

(define box44
	(SetLink
		(VariableNode "$cell_44")
		(VariableNode "$cell_45")
		(VariableNode "$cell_46")
		(VariableNode "$cell_54")
		(VariableNode "$cell_55")
		(VariableNode "$cell_56")
		(VariableNode "$cell_64")
		(VariableNode "$cell_65")
		(VariableNode "$cell_66")
	)
)

(define box47
	(SetLink
		(VariableNode "$cell_47")
		(VariableNode "$cell_48")
		(VariableNode "$cell_49")
		(VariableNode "$cell_57")
		(VariableNode "$cell_58")
		(VariableNode "$cell_59")
		(VariableNode "$cell_67")
		(VariableNode "$cell_68")
		(VariableNode "$cell_69")
	)
)

(define box71
	(SetLink
		(VariableNode "$cell_71")
		(VariableNode "$cell_72")
		(VariableNode "$cell_73")
		(VariableNode "$cell_81")
		(VariableNode "$cell_82")
		(VariableNode "$cell_83")
		(VariableNode "$cell_91")
		(VariableNode "$cell_92")
		(VariableNode "$cell_93")
	)
)

(define box74
	(SetLink
		(VariableNode "$cell_74")
		(VariableNode "$cell_75")
		(VariableNode "$cell_76")
		(VariableNode "$cell_84")
		(VariableNode "$cell_85")
		(VariableNode "$cell_86")
		(VariableNode "$cell_94")
		(VariableNode "$cell_95")
		(VariableNode "$cell_96")
	)
)

(define box77
	(SetLink
		(VariableNode "$cell_77")
		(VariableNode "$cell_78")
		(VariableNode "$cell_79")
		(VariableNode "$cell_87")
		(VariableNode "$cell_88")
		(VariableNode "$cell_89")
		(VariableNode "$cell_97")
		(VariableNode "$cell_98")
		(VariableNode "$cell_99")
	)
)

(define (sudoku-constraints)
	(cells_are_numbers)
	row1
	row2
	row3
	row4
	row5
	row6
	row7
	row8
	row9
	col1
	col2
	col3
	col4
	col5
	col6
	col7
	col8
	col9
	box11
	box14
	box17
	box41
	box44
	box47
	box71
	box74
	box77
)

; Define the variables to be solved for.
(define (variable-decls)
	(ListLink
		(VariableNode "$cell_11")
		(VariableNode "$cell_12")
		(VariableNode "$cell_13")
		(VariableNode "$cell_14")
		(VariableNode "$cell_15")
		(VariableNode "$cell_16")
		(VariableNode "$cell_17")
		(VariableNode "$cell_18")
		(VariableNode "$cell_19")

		(VariableNode "$cell_21")
		(VariableNode "$cell_22")
		(VariableNode "$cell_23")
		(VariableNode "$cell_24")
		(VariableNode "$cell_25")
		(VariableNode "$cell_26")
		(VariableNode "$cell_27")
		(VariableNode "$cell_28")
		(VariableNode "$cell_29")

		(VariableNode "$cell_31")
		(VariableNode "$cell_32")
		(VariableNode "$cell_33")
		(VariableNode "$cell_34")
		(VariableNode "$cell_35")
		(VariableNode "$cell_36")
		(VariableNode "$cell_37")
		(VariableNode "$cell_38")
		(VariableNode "$cell_39")

		(VariableNode "$cell_41")
		(VariableNode "$cell_42")
		(VariableNode "$cell_43")
		(VariableNode "$cell_44")
		(VariableNode "$cell_45")
		(VariableNode "$cell_46")
		(VariableNode "$cell_47")
		(VariableNode "$cell_48")
		(VariableNode "$cell_49")

		(VariableNode "$cell_51")
		(VariableNode "$cell_52")
		(VariableNode "$cell_53")
		(VariableNode "$cell_54")
		(VariableNode "$cell_55")
		(VariableNode "$cell_56")
		(VariableNode "$cell_57")
		(VariableNode "$cell_58")
		(VariableNode "$cell_59")

		(VariableNode "$cell_61")
		(VariableNode "$cell_62")
		(VariableNode "$cell_63")
		(VariableNode "$cell_64")
		(VariableNode "$cell_65")
		(VariableNode "$cell_66")
		(VariableNode "$cell_67")
		(VariableNode "$cell_68")
		(VariableNode "$cell_69")

		(VariableNode "$cell_71")
		(VariableNode "$cell_72")
		(VariableNode "$cell_73")
		(VariableNode "$cell_74")
		(VariableNode "$cell_75")
		(VariableNode "$cell_76")
		(VariableNode "$cell_77")
		(VariableNode "$cell_78")
		(VariableNode "$cell_79")

		(VariableNode "$cell_81")
		(VariableNode "$cell_82")
		(VariableNode "$cell_83")
		(VariableNode "$cell_84")
		(VariableNode "$cell_85")
		(VariableNode "$cell_86")
		(VariableNode "$cell_87")
		(VariableNode "$cell_88")
		(VariableNode "$cell_89")

		(VariableNode "$cell_91")
		(VariableNode "$cell_92")
		(VariableNode "$cell_93")
		(VariableNode "$cell_94")
		(VariableNode "$cell_95")
		(VariableNode "$cell_96")
		(VariableNode "$cell_97")
		(VariableNode "$cell_98")
		(VariableNode "$cell_99")
	)
)


