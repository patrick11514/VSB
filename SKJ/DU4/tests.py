import xml.etree.ElementTree as ET
import copy

import pytest

from tasks import create_student, remove_student, set_task_points, create_task, remove_task

INPUT_XML = '<class class_id="SKJ-2021S"><student student_id="ABC0123"><task task_id="1">4</task><task task_id="2">4</task><task task_id="3">4</task><task task_id="4">0</task><task task_id="5">4</task><task task_id="6">5</task><task task_id="7">5</task><task task_id="8">0</task></student><student student_id="DEF4567"><task task_id="1">4</task><task task_id="2">4</task><task task_id="3">4</task><task task_id="4">4</task><task task_id="5">3</task><task task_id="6">2</task><task task_id="7">1</task><task task_id="8">1</task></student><student student_id="GHI8901"><task task_id="1">1</task><task task_id="2">2</task><task task_id="3">3</task><task task_id="4">2</task><task task_id="5">3</task><task task_id="6">4</task><task task_id="7">4</task><task task_id="8">4</task></student><student student_id="JKL2345"><task task_id="1">4</task><task task_id="2">1</task><task task_id="3">2</task><task task_id="4">5</task><task task_id="5">0</task><task task_id="6">5</task><task task_id="7">5</task><task task_id="8">4</task></student><student student_id="MNO6789"><task task_id="1">2</task><task task_id="2">1</task><task task_id="3">4</task><task task_id="4">0</task><task task_id="5">2</task><task task_id="6">5</task><task task_id="7">2</task><task task_id="8">2</task></student><student student_id="PQR0123"><task task_id="1">0</task><task task_id="2">2</task><task task_id="3">2</task><task task_id="4">3</task><task task_id="5">1</task><task task_id="6">4</task><task task_id="7">1</task><task task_id="8">5</task></student></class>'



def test_create_student():
    root = ET.fromstring(INPUT_XML)
    original_length = len([s.attrib['student_id'] for s in root])

    create_student(root, "TST0000")
    assert len([s.attrib['student_id'] for s in root]) == original_length+1
    assert "TST0000" in [s.attrib['student_id'] for s in root]

    with pytest.raises(Exception, match="^student already exists$"):
        create_student(root, "ABC0123")



def test_remove_student():
    root = ET.fromstring(INPUT_XML)
    original_length = len([s.attrib['student_id'] for s in root])

    remove_student(root, "ABC0123")
    assert len([s.attrib['student_id'] for s in root]) == original_length-1
    assert "ABC0123" not in [s.attrib['student_id'] for s in root]



def test_set_task_points():
    root = ET.fromstring(INPUT_XML)
    assert int(root[0][0].text) == 4
    assert int(root[1][1].text) == 4
    assert int(root[2][2].text) == 3
    assert int(root[3][3].text) == 5
    assert int(root[4][4].text) == 2
    assert int(root[5][5].text) == 4
    assert int(root[0][6].text) == 5
    assert int(root[1][7].text) == 1
    set_task_points(root, 'ABC0123', '1', '5')
    assert int(root[0][0].text) == 5
    assert int(root[1][1].text) == 4
    assert int(root[2][2].text) == 3
    assert int(root[3][3].text) == 5
    assert int(root[4][4].text) == 2
    assert int(root[5][5].text) == 4
    assert int(root[0][6].text) == 5
    assert int(root[1][7].text) == 1
    set_task_points(root, 'DEF4567', '2', '5')
    assert int(root[0][0].text) == 5
    assert int(root[1][1].text) == 5
    assert int(root[2][2].text) == 3
    assert int(root[3][3].text) == 5
    assert int(root[4][4].text) == 2
    assert int(root[5][5].text) == 4
    assert int(root[0][6].text) == 5
    assert int(root[1][7].text) == 1
    set_task_points(root, 'GHI8901', '3', '5')
    assert int(root[0][0].text) == 5
    assert int(root[1][1].text) == 5
    assert int(root[2][2].text) == 5
    assert int(root[3][3].text) == 5
    assert int(root[4][4].text) == 2
    assert int(root[5][5].text) == 4
    assert int(root[0][6].text) == 5
    assert int(root[1][7].text) == 1
    set_task_points(root, 'JKL2345', '4', '0')
    assert int(root[0][0].text) == 5
    assert int(root[1][1].text) == 5
    assert int(root[2][2].text) == 5
    assert int(root[3][3].text) == 0
    assert int(root[4][4].text) == 2
    assert int(root[5][5].text) == 4
    assert int(root[0][6].text) == 5
    assert int(root[1][7].text) == 1
    set_task_points(root, 'MNO6789', '5', '5')
    assert int(root[0][0].text) == 5
    assert int(root[1][1].text) == 5
    assert int(root[2][2].text) == 5
    assert int(root[3][3].text) == 0
    assert int(root[4][4].text) == 5
    assert int(root[5][5].text) == 4
    assert int(root[0][6].text) == 5
    assert int(root[1][7].text) == 1
    set_task_points(root, 'PQR0123', '6', '5')
    assert int(root[0][0].text) == 5
    assert int(root[1][1].text) == 5
    assert int(root[2][2].text) == 5
    assert int(root[3][3].text) == 0
    assert int(root[4][4].text) == 5
    assert int(root[5][5].text) == 5
    assert int(root[0][6].text) == 5
    assert int(root[1][7].text) == 1
    set_task_points(root, 'ABC0123', '7', '0')
    assert int(root[0][0].text) == 5
    assert int(root[1][1].text) == 5
    assert int(root[2][2].text) == 5
    assert int(root[3][3].text) == 0
    assert int(root[4][4].text) == 5
    assert int(root[5][5].text) == 5
    assert int(root[0][6].text) == 0
    assert int(root[1][7].text) == 1
    set_task_points(root, 'DEF4567', '8', '5')
    assert int(root[0][0].text) == 5
    assert int(root[1][1].text) == 5
    assert int(root[2][2].text) == 5
    assert int(root[3][3].text) == 0
    assert int(root[4][4].text) == 5
    assert int(root[5][5].text) == 5
    assert int(root[0][6].text) == 0
    assert int(root[1][7].text) == 5



def test_create_task():
    root = ET.fromstring(INPUT_XML)

    create_task(root, 'PQR0123', '9', '5')
    assert len(root[5]) == 9
    assert root[5][-1].text == '5'
    assert root[5][-1].attrib['task_id'] == '9'

    create_task(root, 'PQR0123', '10', '1')
    assert len(root[5]) == 10
    assert root[5][-1].text == '1'
    assert root[5][-1].attrib['task_id'] == '10'

    with pytest.raises(Exception, match="^task already exists$"):
        create_task(root, 'PQR0123', '1', '50')



def test_remove_task():
    root = ET.fromstring(INPUT_XML)

    assert int(root[0][2].attrib['task_id']) == 3
    remove_task(root, '3')
    assert int(root[0][2].attrib['task_id']) == 4
    assert int(root[0][3].attrib['task_id']) == 5
    remove_task(root, '5')
    assert int(root[0][3].attrib['task_id']) == 6
    assert int(root[0][0].attrib['task_id']) == 1
    remove_task(root, '1')
    assert int(root[0][0].attrib['task_id']) == 2
    remove_task(root, '2')
    assert int(root[0][0].attrib['task_id']) == 4